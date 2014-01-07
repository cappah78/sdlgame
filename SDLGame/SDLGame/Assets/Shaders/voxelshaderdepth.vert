#version 330 core


layout(std140) uniform VoxelTransform2 {
	glm::mat4 matrix;
	glm::vec3 offset;
	glm::mat4 matrix2;
	glm::vec3 offset2;
} transform;

out vec4 pos;

void main(void)
{
	uint index = in_data & INDEX_MASK;
	float z = float((index >> (SIZE_BITS + SIZE_BITS)));
	float y = float((index >> SIZE_BITS) & ((1u << SIZE_BITS) - 1u));
	float x = float(index & ((1u << SIZE_BITS) - 1u));
	pos = transform.matrix * (in_vertex + vec4(x, y, z, 1.0) + vec4(transform.offset, 0.0));
}