#version 330 core

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

in vec4 v_position;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
in vec4 v_color;

layout (location = 0) out vec4 out_color;

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

	out_color = vec4(diffuse, 1.0);
}