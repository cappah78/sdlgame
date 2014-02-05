#version 330 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec4 in_color;
layout(location = 4) in vec3 in_tangent;
layout(location = 5) in vec3 in_bitangent;

uniform mat4 u_mvp;
uniform mat4 u_transform;

out vec4 v_position;
out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
out vec4 v_color;

void main()
{
	vec4 pos = u_transform * in_position;
	gl_Position = u_mvp * pos;
	v_position = pos;
	v_texcoord = in_texcoord;
	v_normal = in_normal;
	v_tangent = in_tangent;
	v_bitangent = in_bitangent;
	v_color = in_color;
}