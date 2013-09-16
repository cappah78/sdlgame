#version 330 core

uniform sampler2D u_positionBuffer;
uniform sampler2D u_normalBuffer;
uniform sampler2D u_depthBuffer;
uniform sampler2DArrayShadow u_shadowMapArray;

layout(std140) uniform transformBlock {
	mat4 MVPMatrix;
	mat4 MVMatrix;
	mat4 PMatrix;
} Transform;


layout(location = 0) out vec4 color;

in vec2 texCoord;

float linearizeDepth(float zoverw)
{
	const float n = 0.1; // camera z near
	const float f = 600.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

vec3 reconstructPosition(in float p_depth, in vec2 p_ndc, in vec4 p_projParams)
{ 
    float depth = p_depth * 2.0f - 1.0f;
    float viewDepth = p_projParams.w / (depth - p_projParams.z);

    return vec3((p_ndc * viewDepth) / p_projParams.xy, viewDepth);
}

void main()
{
	float depthCol = texture(u_depthBuffer, texCoord).r;
	float depth = linearizeDepth(depthCol);

	vec3 posCol = reconstructPosition(depthCol, texCoord.xy * 2.0f - 1.0f, ProjParams.params);

	//vec4 posCol = texture(u_positionBuffer, texCoord);
	color = vec4(posCol, 1.0);
	//vec4 norColr = texture(u_normalBuffer, texCoord);


	//color = vec4(depth, depth, depth, 1.0);
	//color = vec4(norColr.xyz, 1.0);
}

