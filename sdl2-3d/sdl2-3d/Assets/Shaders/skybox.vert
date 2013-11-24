#version 330 core

layout (location = 0) in vec4 in_position;

uniform mat4 u_mvp;

out vec3 position;

void main()
{
	gl_Position = u_mvp * in_position;
	position = vec3(in_position);
}