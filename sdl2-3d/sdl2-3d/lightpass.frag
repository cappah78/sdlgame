#version 330 core

uniform sampler2D u_positionBuffer;
uniform sampler2D u_normalBuffer;
uniform sampler2D u_depthBuffer;
uniform sampler2DArrayShadow u_shadowMapArray;

layout(location = 0) out vec4 color;

in vec2 texCoord;

float linearizeDepth(float zoverw)
{
	const float n = 0.1; // camera z near
	const float f = 600.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{
	float depthCol = texture(u_depthBuffer, texCoord).r;
	float depth = linearizeDepth(depthCol);


	//color = vec4(posCol.xyz, 1.0);
	vec4 norColr = texture(u_normalBuffer, texCoord);
	vec4 posCol = texture(u_positionBuffer, texCoord);

	color = vec4(depth, depth, depth, 1.0);
	//color = vec4(norColr.xyz, 1.0);
}

