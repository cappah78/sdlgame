#version 330 core

#extension GL_EXT_texture_array : enable

layout(std140) uniform VoxelTransform {
	mat4 MVPMatrix;
	vec3 normal;
} transform;

uniform sampler2DArray texArr;

in vec3 texCoord;

out vec4 color;

void main()
{
	color = texture2DArray(texArr, texCoord);
}