#version 330 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_normal;

uniform mat4 u_mvp;

out vec2 texCoord;

void main()
{
	gl_Position = u_mvp * in_position;
	texCoord = in_texCoord;
}