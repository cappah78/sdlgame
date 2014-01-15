#version 330

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texCoord;

out vec2 texCoord;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * in_position;
	texCoord = in_texCoord;
}
