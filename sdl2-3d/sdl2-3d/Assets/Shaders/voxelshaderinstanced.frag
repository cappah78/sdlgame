#version 330 core

#extension GL_EXT_texture_array : enable

uniform sampler2DArray texArr;

in vec3 texCoord;
in vec4 color;

out vec4 out_color;

void main()
{
	vec4 texCol = texture2DArray(texArr, texCoord);
	out_color = vec4((texCol.rgb + color.rgb) * color.a , texCol.a);
}