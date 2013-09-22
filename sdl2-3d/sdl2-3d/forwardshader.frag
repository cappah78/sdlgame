#version 330 core

const int maxLights = 10;
const float shininess = 0.001;
const float strength = 0.01;

struct Light
{
	vec4 position;
	vec4 direction;
	vec4 color;
};

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

layout(std140) uniform LightData {
	Light lights[maxLights];
} lightData;

layout(std140) uniform LightTransform {
	mat4 VPMatrix[maxLights];
} lightTransforms;

uniform sampler2D tex;
uniform sampler2DArrayShadow u_shadowMapArray;

uniform int u_numLights;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

layout(location = 0) out vec4 color;

void main()
{
	vec3 texCol = texture(tex, texCoord).rgb;
	vec3 eyeDirection = -vec3(camera.VMatrix[2][0], camera.VMatrix[2][1], camera.VMatrix[2][2]);

	vec3 scatteredLight = vec3(0);
	vec3 reflectedLight = vec3(0);

	for (int i = 0; i < u_numLights; ++i)
	{
		float spotCosCutoff = lightData.lights[i].position.w;
		if (spotCosCutoff >= 1.0f)
			continue;

		vec3 lightPosition = lightData.lights[i].position.xyz;
		vec3 lightDirection = lightData.lights[i].direction.xyz;
		vec3 lightVec = lightPosition.xyz - position.xyz;
		float lightDistance = length(lightVec);
		lightVec /= lightDistance;

		float spotCos = dot(lightVec, normalize(-lightDirection));
		if (spotCos < spotCosCutoff)
			continue;

		float spotDropoff = lightData.lights[i].direction.w;
		float linearAtt = lightData.lights[i].color.w;
		vec3 lightColor = lightData.lights[i].color.xyz;

		float att = 1.f / ( 1.0f + linearAtt * lightDistance);
		att *= pow(spotCos, spotDropoff);

		vec3 halfVector = normalize(lightVec + eyeDirection);
		float diffuse = max(0.0, dot(normal, lightVec));
		float specular = max(0.0, dot(normal, halfVector));
		specular = diffuse == 0.0 ? 0.0 : pow(specular, shininess) * strength;
		
		vec4 shadowCoordHom = lightTransforms.VPMatrix[i] * vec4(position, 1.0f);
		shadowCoordHom.xyz /= shadowCoordHom.w;
		shadowCoordHom.xyz = shadowCoordHom.xyz * 0.5 + 0.5;
		shadowCoordHom.z *= 0.9999;
		vec4 shadowCoord = vec4(shadowCoordHom.x, shadowCoordHom.y, i, shadowCoordHom.z);
		float shadow = texture(u_shadowMapArray, shadowCoord);
		float counterShadow = min(floor(shadowCoord.w), 1.f);
		shadow = mix(shadow, 1.f, counterShadow);

		// Accumulate all the lights’ effects
		scatteredLight += vec3(lightColor) * att * diffuse * shadow;
		reflectedLight += vec3(lightColor) * att * specular * shadow;
	}

	vec3 rgb = min(texCol * scatteredLight + reflectedLight, vec3(1.0));
	color = vec4(rgb, 1.0);
}

