#version 330 core

uniform sampler2D tex;

layout (location = 0) out vec4 color;

in vec2 texCoord;

void main()
{
	color = texture(tex, texCoord);
}