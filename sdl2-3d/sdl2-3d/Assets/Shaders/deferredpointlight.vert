#version 330 core

uniform mat4 u_mvp;
uniform mat4 u_worldTrans;

layout (location = 0) in vec3 in_position;

void main()
{
	vec4 pos = u_worldTrans * vec4(in_position, 1.0);
	gl_Position = u_mvp * pos;
	//gl_Position = u_mvp * vec4(in_position, 1.0);
}