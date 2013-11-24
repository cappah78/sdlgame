#version 330

layout (location = 0) out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	color = texture(tex, texCoord);
}
