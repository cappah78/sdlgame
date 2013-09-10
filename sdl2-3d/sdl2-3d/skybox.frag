#version 330 core

layout (location = 0) out vec4 color;

in vec3 vertexVector;

uniform samplerCube cubeMap;

void main()
{
	color = texture(cubeMap, vertexVector);
}