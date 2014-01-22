#version 330 core

uniform sampler2D tex;

layout (location = 0) out vec4 color;

in vec2 texCoord;

void main()
{
	float depth = texture(tex, texCoord).r;

	float f = 450.0;
	float n = 0.5;
	float z = (2 * n) / (f + n - depth * (f - n));

	color = vec4(vec3(depth), 1.0);
}