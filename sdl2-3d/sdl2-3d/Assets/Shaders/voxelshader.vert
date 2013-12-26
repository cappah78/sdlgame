#version 330 core

const unsigned int POSITION_BITS = 5u; // sqrt(chunkSize) + 1
const unsigned int POSITION_BITS_2 = POSITION_BITS * 2u;
const unsigned int POSITION_BITS_3 = POSITION_BITS * 3u;
const unsigned int POSITION_BITS_BITMASK_OFFSET = ((1u << POSITION_BITS) - 1u);

const unsigned int TEXTURE_ID_BITS = 10u;
const unsigned int POSITION_BITS_MASK = 0x00007FFFu;
const unsigned int TEXTURE_ID_MASK = 0x01FF8000u;
const unsigned int UNUSED_BITS_MASK = 0xFE000000u;

uniform mat4 u_mvp;
uniform vec3 u_normal;

layout(location = 0) in uint in_point;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec4 in_color;

out vec3 texCoord;
out vec4 color;

void main(void)
{
	// grab the position bits from the data
	uint index = in_point & POSITION_BITS_MASK;

	// extract the position using black magic
	float z = float(index >> POSITION_BITS_2);
	float y = float((index >> POSITION_BITS) & POSITION_BITS_BITMASK_OFFSET);
	float x = float(index & POSITION_BITS_BITMASK_OFFSET);

	//extract textureId using black magic
	float textureId = float((in_point & TEXTURE_ID_MASK) >> POSITION_BITS_3);

	// offset the vertices by the position
	gl_Position = u_mvp * vec4(x, y, z, 1.0);

	// supply 3d texcoord to fragment shader
	texCoord = vec3(in_texCoord, textureId);
	color = in_color;
}