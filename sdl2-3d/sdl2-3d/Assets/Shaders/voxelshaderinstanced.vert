#version 330 core

const unsigned int POSITION_BITS = 4u; // sqrt(CHUNK_SIZE), must be power of 2.
const unsigned int POSITION_BITS_2 = POSITION_BITS * 2u;
const unsigned int POSITION_BITS_3 = POSITION_BITS * 3u;
const unsigned int POSITION_BITS_BITMASK_OFFSET = ((1u << POSITION_BITS) - 1u);

const unsigned int TEXTURE_ID_BITS = 12u;
const unsigned int POSITION_BITS_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;
const unsigned int UNUSED_BITS_MASK = 0xFF000000u;

uniform mat4 u_mvp;
uniform vec3 u_normal;

layout(location = 0) in vec4 in_vertex;		// vertices reused for every quad
layout(location = 1) in vec2 in_texCoord;	// texcoords reused for every quad
layout(location = 2) in uint in_data;		// data containing texture id and position, unique for every block
// additional per vertex properties like color for lighting can be added easily


out vec3 texCoord;

void main(void)
{
	// grab the position bits from the data
	uint index = in_data & POSITION_BITS_MASK;

	// extract the position using black magic
	float z = float(index >> POSITION_BITS_2);
	float y = float((index >> POSITION_BITS) & POSITION_BITS_BITMASK_OFFSET);
	float x = float(index & POSITION_BITS_BITMASK_OFFSET);

	// offset the vertices by the position
	gl_Position = u_mvp * (in_vertex + vec4(x, y, z, 1.0));

	//extract textureId using black magic
	float textureId = float((in_data & TEXTURE_ID_MASK) >> POSITION_BITS_3);

	// supply 3d texcoord to fragment shader
	texCoord = vec3(in_texCoord, textureId);
}