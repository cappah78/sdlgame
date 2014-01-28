#version 330

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texcoord;


void main()
{
	gl_Position = in_position;
}
