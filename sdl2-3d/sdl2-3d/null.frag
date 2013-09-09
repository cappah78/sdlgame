#version 330 core

in (location = 0) vec4 in_position;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * in_position;
}