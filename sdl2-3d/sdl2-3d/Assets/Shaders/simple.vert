#version 330 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;

out vec4 color;

void main()
{
	gl_Position = in_position;
	color = in_color;
}