#version 330 core

const int maxLights = 10;
const float shininess = 0.01;
const float strength = 0.01;

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_normal;

struct LightProperties {
	vec4 position;
	vec4 color;
	mat4 lightMvp;
};

layout(std140) uniform LightBlock
{
	vec3 eyeDirection;
	LightProperties lights[maxLights];
} u_lightData;

uniform mat4 u_mvp;
uniform mat4 u_mv;

out vec2 texCoord;
out vec3 scatteredLight;
out vec3 reflectedLight;

void computeLight(in LightProperties light, in vec3 normal, in vec4 position)
{
	if (light.position.w == 0.0)
		return;

	vec3 lightDirection = vec3(light.position);
	float attenuation = 1.0;

	lightDirection = lightDirection - vec3(position);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;
	/*
	attenuation = 1.0 /
		(light.constantAttenuation
		+ light.linearAttenuation * lightDistance
		+ light.quadraticAttenuation * lightDistance * lightDistance);
	*/

	attenuation = 1.0 / (light.color.a * lightDistance);

	vec3 halfVector = normalize(lightDirection + u_lightData.eyeDirection);

	float diffuse = max(0.0, dot(normal, lightDirection));
	float specular = max(0.0, dot(normal, halfVector));

	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, shininess) * strength;

	// Accumulate all the lights’ effects
	scatteredLight += vec3(light.color) * diffuse * attenuation;
	reflectedLight += vec3(light.color) * specular * attenuation;
}

void main()
{
	vec3 normal = normalize(mat3(u_mv) * in_normal);
	vec4 position = vec4(u_mv * in_position);

	scatteredLight = vec3(0.01);
	reflectedLight = vec3(0.0);
	
	// loop over all the lights
	for (int i = 0; i < maxLights; ++i) 
	{
		computeLight(u_lightData.lights[i], normal, position);
	}
	
	texCoord = in_texCoord;
	gl_Position = u_mvp * in_position;
}
