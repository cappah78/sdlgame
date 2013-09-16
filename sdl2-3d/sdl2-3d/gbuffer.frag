#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;

layout(location = 0) out vec4 outPositionBuffer;
layout(location = 1) out vec3 outNormalBuffer;

void main(void) {

	/* output position */
	outPositionBuffer = vec4(vertexPosition, 1.f);

	/* output renormalized normal */
	outNormalBuffer = normalize(vertexNormal);;
}