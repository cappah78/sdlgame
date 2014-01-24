#version 330 core

const uint POSITION_BITS = 5u; // sqrt(chunkSize) + 1
const uint POSITION_BITS_2 = POSITION_BITS * 2u;
const uint POSITION_BITS_3 = POSITION_BITS * 3u;
const uint POSITION_BITS_BITMASK_OFFSET = ((1u << POSITION_BITS) - 1u);

const uint TEXTURE_ID_BITS = 10u;
const uint POSITION_BITS_MASK = 0x00007FFFu;
const uint TEXTURE_ID_MASK = 0x01FF8000u;
const uint UNUSED_BITS_MASK = 0xFE000000u;

uniform vec3 u_camPos;
uniform vec3 u_chunkOffset;
uniform vec2 u_screenSize;
uniform vec2 u_renderOffset;

layout(location = 0) in uint in_point;
//layout(location = 1) in vec2 in_texCoord;
//layout(location = 2) in vec4 in_color;
layout(location = 3) in uint in_vertexId;

out vec4 color;
out float eyeDist;

void main(void)
{
	// grab the position bits from the data
	uint index = in_point & POSITION_BITS_MASK;

	// extract the position using black magic
	float z = float(index >> POSITION_BITS_2);
	float y = float((index >> POSITION_BITS) & POSITION_BITS_BITMASK_OFFSET);
	float x = float(index & POSITION_BITS_BITMASK_OFFSET);
	vec3 position = vec3(x, y, z) + u_chunkOffset;

	float dst = sqrt(
		(position.x - u_camPos.x) * (position.x - u_camPos.x) +
        (position.y - u_camPos.y) * (position.y - u_camPos.y) +
        (position.z - u_camPos.z) * (position.z - u_camPos.z)); 
	eyeDist = dst;

	vec4 renderPos;
	renderPos.x = (float(in_vertexId % 256u) + 0.5) / u_screenSize.x - 0.5;
	renderPos.y = (float(in_vertexId / 256u) + 0.5) / u_screenSize.y - 0.5;
	renderPos.x += (u_renderOffset.x) / u_screenSize.x;
	renderPos.y += (u_renderOffset.y) / u_screenSize.y;
	renderPos.z = 0.0;
	renderPos.w = 0.5;
	gl_Position = renderPos;
}