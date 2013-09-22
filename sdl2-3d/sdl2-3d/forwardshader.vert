#version 330 core

layout(std140) uniform CameraTransform {
	mat4 VPMatrix;
	mat4 VMatrix;
	mat4 PMatrix;
} camera;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_normal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

void main(void)
{
	position = in_position;
	normal = in_normal;
	texCoord = in_texCoord;
	gl_Position = camera.VPMatrix * vec4(in_position, 1.0f);
}