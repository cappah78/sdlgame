#version 330 core

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

uniform mat4 u_mvp;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_normal;

out vec2 vertexTexCoord;
out vec3 vertexNormal;

out gl_PerVertex {
	vec4 gl_Position;
};

void main(void)
{
	vertexTexCoord = in_texCoord;
	vertexNormal = in_normal;

	/* transform position into clip space */
	gl_Position = camera.VPMatrix * vec4(in_position, 1);
}