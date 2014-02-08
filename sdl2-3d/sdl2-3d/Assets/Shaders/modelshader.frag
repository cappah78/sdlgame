#version 330 core

#define MAX_LIGHTS 1024
#define LIGHT_GRID_MAX_X 32
#define LIGHT_GRID_MAX_Y 32

uniform sampler2D u_diffuseTex;
uniform sampler2D u_normalTex;
uniform sampler2D u_specularTex;
uniform sampler2D u_opacityTex;

layout(std140) uniform MaterialProperties
{
	vec3 u_materialDiffuse; 
	float u_materialAlpha;
	vec3 u_materialSpecular; 
	vec3 u_materialEmissive; 
	float u_materialSpecExp;
};

layout(std140) uniform Globals
{
	mat4 viewMatrix; 
	mat4 viewProjectionMatrix; 
	mat4 inverseProjectionMatrix;
	mat4 normalMatrix; 
	vec3 worldUpDirection;
	vec3 ambientGlobal;
	vec2 invFbSize;
	ivec2 fbSize;
	float recNear; /**< 1.0f / g_near */
	float recLogSD1; /**< 1.0f / logf(sD + 1.0f), used to compute cluster index. */
};

uniform LightGrid { ivec4 u_lightGridCountOffsets[LIGHT_GRID_MAX_X * LIGHT_GRID_MAX_Y]; };

uniform isamplerBuffer u_tileLightIndexListTex;
uniform LightPositionRanges { vec4 u_lightPositionRange[MAX_LIGHTS]; };
uniform LightColors { vec4 u_lightColors[MAX_LIGHTS]; };

in vec4 v_position;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
in vec4 v_color;

layout (location = 0) out vec4 out_color;

vec3 doLight(vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess, vec3 viewDir, vec3 lightPos, vec3 lightColor, float range)
{
	vec3 lightDir = vec3(lightPos) - position;
	float dist = length(lightDir);
	lightDir = normalize(lightDir);

	float inner = 0.0;
	float ndotL = max(dot(normal, lightDir),0.0);
	float att = max(1.0 - max(0.0, (dist - inner) / (range - inner)), 0.0);
	float normalizationFactor = ((shininess + 2.0) / 8.0);

	vec3 h = normalize(lightDir + viewDir);
	vec3 spec = specular * pow(max(0, dot(h, normal)), shininess) * normalizationFactor;

	return ndotL * att * lightColor * (diffuse + spec);
}

vec3 doLight(vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess, vec3 viewDir, int lightIndex)
{
	vec3 lightPos = u_lightPositionRange[lightIndex].xyz;
	float lightRange = u_lightPositionRange[lightIndex].w;
	vec3 lightColor = u_lightColors[lightIndex].xyz;

	return doLight(position, normal, diffuse, specular, shininess, viewDir, lightPos, lightColor, lightRange);
}

// computes tiled lighting for the current fragment, using the built in gl_FragCoord to determine the correct tile.
vec3 evalTiledLighting(in vec3 diffuse, in vec3 specular, in float shininess, in vec3 position, in vec3 normal, in vec3 viewDir)
{
	ivec2 l = ivec2(int(gl_FragCoord.x) / LIGHT_GRID_MAX_X, int(gl_FragCoord.y) / LIGHT_GRID_MAX_Y);
	int lightCount = u_lightGridCountOffsets[l.x + l.y * LIGHT_GRID_MAX_X].x;
	int lightOffset = u_lightGridCountOffsets[l.x + l.y * LIGHT_GRID_MAX_X].y;

	vec3 color = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < lightCount; ++i)
	{
		int lightIndex = texelFetch(u_tileLightIndexListTex, lightOffset + i).x; 
		color += doLight(position, normal, diffuse, specular, shininess, viewDir, lightIndex);
	}
  
	return color;
}

vec3 toSrgb(vec3 color)
{
  return pow(color, vec3(1.0 / 2.2));
}

void main()
{
	if (texture2D(u_opacityTex, v_texcoord).r < 0.5)
	{
		discard;
	}

	vec3 position = vec3(v_position);
	vec3 viewDir = -normalize(position);
	vec3 normalSpaceX = normalize(v_tangent);
	vec3 normalSpaceY = normalize(v_bitangent);
	vec3 normalSpaceZ = normalize(v_normal);
  
	vec3 normalMapSample = texture2D(u_normalTex, v_texcoord).xyz * vec3(2.0) - vec3(1.0);
  
	vec3 normal = normalize(normalMapSample.x * normalSpaceX + normalMapSample.y * normalSpaceY + normalMapSample.z * normalSpaceZ);
	vec3 diffuse = texture2D(u_diffuseTex, v_texcoord).rgb * u_materialDiffuse;
	vec3 specular = texture2D(u_specularTex, v_texcoord).rgb * u_materialSpecular;
	const float amb = 0.5;
	vec3 ambient = diffuse * amb;
  
	float fresnelTerm = pow(clamp(1.0 + dot(-viewDir, normal), 0.0, 1.0), 5.0);
	vec3 fresnelSpec = specular + (vec3(1.0) - specular) * fresnelTerm;
	vec3 lighting = evalTiledLighting(diffuse, fresnelSpec, u_materialSpecular, position, normal, viewDir);

	out_color = vec4(toSrgb(lighting + ambient), u_materialAlpha + (1.0 - u_materialAlpha) * fresnelTerm);
}