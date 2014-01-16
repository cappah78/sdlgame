#version 330 core

in vec2 texCoord;
out vec4 out_color;

void main(void)
{
	out_color = vec4(texCoord, 1.0, 1.0);
}