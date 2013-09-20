#version 330 core

layout(location = 0) in vec3 in_vertexPosition;

out vec3 vertexPosition;

void main(void) 
{
	vertexPosition = in_vertexPosition;
}