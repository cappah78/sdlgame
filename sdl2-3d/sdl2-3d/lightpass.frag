#version 330 core

uniform sampler2D u_colorBuffer;
uniform sampler2D u_normalBuffer;
uniform sampler2D u_depthBuffer;
uniform sampler2DArrayShadow u_shadowMapArray;

layout(std140) uniform transformBlock {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} Camera;

layout(location = 0) out vec4 color;

in vec2 texCoord;

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
	vec4 world = inverse(Camera.VPMatrix) * vec4(screen, depth, 1.0);
	
	vec3 result = world.xyz / world.w;
	result.z = -result.z;
	return result;
}

void main()
{
	color = texture(u_colorBuffer, texCoord);
}

