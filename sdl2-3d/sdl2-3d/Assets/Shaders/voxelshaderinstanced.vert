#version 330 core

const unsigned int SIZE_BITS = 4u; //2^sizebits == chunksize or sqrt(chunkSize)
const unsigned int INDEX_BITS = 3u * SIZE_BITS;
const unsigned int TEXTURE_ID_BITS = 12u;
const unsigned int OCCLUSION_BITS = 8u;

const unsigned int INDEX_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;
const unsigned int OCCLUSION_MASK = 0xFF000000u;

layout(location = 0) in uint in_pointData;

out vec4 pos;
out uint occlusionBits;
out float texId;

void main(void)
{
	uint index = in_pointData & INDEX_MASK;
	occlusionBits = in_pointData & OCCLUSION_MASK;
	texId = float((in_pointData & TEXTURE_ID_MASK) >> INDEX_BITS);

	float z = float((index >> (SIZE_BITS + SIZE_BITS)));
	float y = float((index >> SIZE_BITS) & ((1u << SIZE_BITS) - 1u));
	float x = float(index & ((1u << SIZE_BITS) - 1u));

	pos = vec4(x, y, z, 1.0);
}