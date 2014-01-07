#version 330 core

//TODO: write #include parser to build shader
//TODO: append #define to shader using code.

#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS 5
#define MAX_DIRECTIONAL_LIGHTS 3

#define LIGHT_RANGE_SCALE_MULT 1000.0f

//TODO: Material stuff, properties per material.
const float shininess = 0.001;
const float strength = 0.01;

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

uniform sampler2D tex;
uniform vec3 u_ambient;

struct PointLight
{
	vec3 position;
	vec3 color;	//just using rgb, but vec4 for non confusing vec3 padding
};
layout(std140) uniform PointLights
{
	PointLight lights[MAX_POINT_LIGHTS];
} pointLights;

struct SpotLight
{
	vec4 positionAndAngle;
	vec4 directionAndDropoff;
	vec3 color;
};
layout(std140) uniform SpotLights
{
	SpotLight lights[MAX_SPOT_LIGHTS];
} spotLights;

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};
layout(std140) uniform DirectionalLights
{
	DirectionalLight lights[MAX_DIRECTIONAL_LIGHTS];
} directionalLights;


in vec3 position;
in vec2 texCoord;
in vec3 normal;

layout(location = 0) out vec4 color;

vec3 lightLevel;
vec3 eyeDirection;

void applyPointLights()
{
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		PointLight light = pointLights.lights[i];

		vec3 lightDir = light.position - position;

		float sqrDist = dot(lightDir, lightDir);
		float invDist = inversesqrt(sqrDist);
		lightDir *= invDist;
		float dist = invDist * sqrDist;

		float att = 1.0 / (1.0 + dist);
		float NdotL = clamp(dot(normal, lightDir), 0.0, 2.0);

		lightLevel += light.color * att * NdotL;
	}
}

void applySpotLights()
{
	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
	{
		SpotLight light = spotLights.lights[i];
		vec3 lightDir = light.positionAndAngle.xyz - position;

		float sqrDist = dot(lightDir, lightDir);
		float invDist = inversesqrt(sqrDist);
		float dist = invDist * sqrDist;
		lightDir *= invDist;

		float att = 1.0 / (1.0 + dist);
		float NdotL = clamp(dot(normal, lightDir), 0.0, 2.0);
		float spotCos = dot(lightDir, -light.directionAndDropoff.xyz);

		float dropoff = light.directionAndDropoff.w;	

		if (spotCos < light.positionAndAngle.w)
			att = 0.0;
		else
			att *= pow(spotCos, clamp(dropoff, 1.0, 15.0)); //what the fuck.

		lightLevel += light.color * att * NdotL;
	}
}

void applyDirectionalLights()
{
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i)
	{
		DirectionalLight light = directionalLights.lights[i];
		float NdotL = max(dot(normal, -light.direction), 0.0);
		lightLevel += light.color * NdotL;
	}
}

void main()
{
	vec3 texCol = texture(tex, texCoord).rgb;

	eyeDirection = -vec3(camera.VMatrix[2][0], camera.VMatrix[2][1], camera.VMatrix[2][2]);
	lightLevel = vec3(0);

	applyPointLights();
	applySpotLights();
	applyDirectionalLights();
	lightLevel += u_ambient;

	vec3 rgb = min(texCol * lightLevel, vec3(1.0));
	color = vec4(rgb, 1.0);
}



