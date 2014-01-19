#version 330 core

const uint TEXTURE_ID_BITS = 10u;
const uint POSITION_BITS_1 = 5u; // sqrt(chunkSize) + 1
const uint POSITION_BITS_2 = POSITION_BITS_1 * 2u;
const uint POSITION_BITS_3 = POSITION_BITS_1 * 3u;
const uint POSITION_BITS_BITMASK_OFFSET = ((1u << POSITION_BITS_1) - 1u);

const uint POSITION_BITS_MASK = 0x00007FFFu;
const uint TEXTURE_ID_MASK = 0x01FF8000u;
const uint UNUSED_BITS_MASK = 0xFE000000u;

uniform mat4 u_mvp;
uniform vec3 u_camPos;
uniform vec3 u_normal;
uniform vec3 u_chunkOffset;

// Bit layout: u = unused, i = textureid, x/y/z = pos within chunk //
// uuuuuuuiiiiiiiiiizzzzzyyyyyxxxxx	//
layout(location = 0) in uint in_point;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec4 in_color;

out vec3 texCoord;
out vec4 color;
out float eyeDist;

void main(void)
{
	// grab the position bits from the data
	uint index = in_point & POSITION_BITS_MASK;

	// extract the position using black magic
	float z = float(index >> POSITION_BITS_2);
	float y = float((index >> POSITION_BITS_1) & POSITION_BITS_BITMASK_OFFSET);
	float x = float(index & POSITION_BITS_BITMASK_OFFSET);

	//extract textureId using black magic
	float textureId = float((in_point & TEXTURE_ID_MASK) >> POSITION_BITS_3);

	vec3 position = vec3(x, y, z) + u_chunkOffset;

	// offset the vertices by the position
	gl_Position = u_mvp * vec4(position, 1.0);

	eyeDist = length(position - u_camPos); 

	// supply 3d texcoord to fragment shader
	texCoord = vec3(in_texCoord, textureId);
	color = in_color;
}