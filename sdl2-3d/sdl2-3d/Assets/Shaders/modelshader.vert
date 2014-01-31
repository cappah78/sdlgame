#version 330 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec4 in_color;
layout(location = 4) in vec3 in_tangent;
layout(location = 5) in vec3 in_bitangent;

uniform mat4 u_mvp;
uniform mat4 u_transform;

out vec4 position;
out vec2 texcoord;

void main()
{
	vec4 pos = u_transform * in_position;
	gl_Position = u_mvp * pos;
	position = pos;
	texcoord = in_texcoord;
}