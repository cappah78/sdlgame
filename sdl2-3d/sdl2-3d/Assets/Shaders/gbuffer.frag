#version 330 core

in vec3 texCoord;

out vec3 out_texCoord;

void main()
{
	out_texCoord = texCoord;
}