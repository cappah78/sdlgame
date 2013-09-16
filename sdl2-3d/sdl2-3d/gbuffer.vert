#version 330 core

layout(std140) uniform transformBlock {
	mat4 MVPMatrix;
	mat4 MVMatrix;
	mat4 PMatrix;
} Transform;

uniform mat4 u_mvp;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec3 vertexPosition;
out vec3 vertexNormal;

out gl_PerVertex {
	vec4 gl_Position;
};

void main(void) {

	/* pass through world space position */
	vertexPosition = in_position;

	/* pass through world space normal */
	vertexNormal = in_normal;

	/* transform position into clip space */
	gl_Position = Transform.MVPMatrix * vec4(in_position, 1);
}