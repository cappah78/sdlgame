#version 430 core

layout(binding = 0) uniform sampler2D positionBuffer;
layout(binding = 1) uniform sampler2D normalBuffer;
layout(binding = 2) uniform sampler2D depthBuffer;
layout(binding = 3) uniform sampler2DArrayShadow shadowMapArray;

layout(location = 0) out vec4 color;
layout(location = 0) in vec2 texCoord;

layout(std140, binding = 0) uniform transform {
	mat4 MVPMatrix;
} Transform;

float linearizeDepth(float zoverw)
{
		const float n = 0.1; // camera z near
		const float f = 600.0; // camera z far
		return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{
	float depthCol = texture(depthBuffer, texCoord).r;
	float depth = linearizeDepth(depthCol);

	vec4 posCol = texture(positionBuffer, texCoord);

	color = vec4(depth, depth, depth, 1.0);
}

