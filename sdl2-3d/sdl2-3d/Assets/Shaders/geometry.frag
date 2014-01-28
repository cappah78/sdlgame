#version 330 core

uniform sampler2DArray texArr;
in vec3 texCoord;
in vec4 color;

out vec3 out_color;

void main()
{
	vec4 texCol = texture(texArr, texCoord);
	out_color = vec3((texCol.rgb + color.rgb) * color.a);
}