#version 330 core

in vec2 vertexTexCoord;
in vec3 vertexNormal;

uniform sampler2D tex;

layout(location = 0) out vec4 outColorBuffer;
layout(location = 1) out vec3 outNormalBuffer;

void main(void) {
	/* output color */
	outColorBuffer = texture(tex, vertexTexCoord).rgba;

	/* output renormalized normal */
	outNormalBuffer = normalize(vertexNormal);
}