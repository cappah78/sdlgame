#version 330 core

layout (location = 0) in vec4 in_position;

uniform mat4 u_mv;
uniform mat4 u_mvp;

out vec3 vertexVector;

void main()
{
	vec3 cameraPosition = -u_mv[3].xyz * mat3(u_mv);
	gl_Position = u_mvp * (in_position + vec4(cameraPosition, 0.0));
	vertexVector = vec3(in_position);
}