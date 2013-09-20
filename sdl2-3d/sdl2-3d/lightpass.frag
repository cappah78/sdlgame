#version 330 core

const int maxLights = 100;
const float shininess = 0.001;
const float strength = 0.01;

uniform sampler2D u_colorBuffer;
uniform sampler2D u_normalBuffer;
uniform sampler2D u_depthBuffer;
uniform sampler2DArrayShadow u_shadowMapArray;

uniform int u_numLights;

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

struct LightType
{
	vec4 position;
	vec4 direction;
	vec4 color;
};

layout(std140) uniform LightData {
	LightType lights[maxLights];
} lightData;

layout(std140) uniform LightTransform {
	mat4 VPMatrix[maxLights];
} lightTransforms;

in vec2 texCoord;

layout(location = 0) out vec4 color;

float linearizeDepth(float zoverw)
{
	const float n = 0.5; // camera z near
	const float f = 600.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

vec3 depthToPos(vec2 texcoord)
{
	vec2 screen = texcoord;
	float depth = texture(u_depthBuffer, screen).x;
	screen.x = (screen.x * 2.0) - 1.0;
	screen.y = (screen.y * 2.0) - 1.0;
	depth = (depth * 2.0) - 1.0;
	vec4 world = inverse(camera.VPMatrix) * vec4(screen, depth, 1.0);
	
	vec3 result = world.xyz / world.w;
	return result;
}


void main()
{
	vec3 normal = texture(u_normalBuffer, texCoord).xyz;
	vec3 position = depthToPos(texCoord);
	vec3 texCol = texture(u_colorBuffer, texCoord).rgb;
	vec3 eyeDirection = -vec3(camera.VMatrix[2][0], camera.VMatrix[2][1], camera.VMatrix[2][2]);
	vec3 scatteredLight = vec3(0);
	vec3 reflectedLight = vec3(0);
	
	for (int i = 0; i < u_numLights; ++i)
	{
		float spotCosCutoff = lightData.lights[i].position.w;
		if (spotCosCutoff >= 1.0f)
			continue;
		float spotDropoff = lightData.lights[i].direction.w;
		float linearAtt = lightData.lights[i].color.w;
		vec3 lightPosition = lightData.lights[i].position.xyz;
		vec3 lightDirection = lightData.lights[i].direction.xyz;
		vec3 lightColor = lightData.lights[i].color.xyz;

		vec3 lightVec = lightPosition.xyz - position.xyz;
		float lightDistance = length(lightVec);
		lightVec /= lightDistance;

		float spotCos = dot(lightVec, normalize(-lightDirection));

		if (spotCos < spotCosCutoff)
			continue;

		float att = 1.f / ( 1.0f + linearAtt * lightDistance);
		att *= pow(spotCos, spotDropoff);

		vec3 halfVector = normalize(lightVec + eyeDirection);

		float diffuse = max(0.0, dot(normal, lightVec));
		float specular = max(0.0, dot(normal, halfVector));
		specular = diffuse == 0.0 ? 0.0 : pow(specular, shininess) * strength;

		// Accumulate all the lights’ effects
		scatteredLight += vec3(lightColor) * att * diffuse;
		reflectedLight += vec3(lightColor) * att * specular;
	}

	vec3 rgb = min(texCol * scatteredLight + reflectedLight, vec3(1.0));
	color = vec4(rgb, 1.0);
}

