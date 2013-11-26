#version 330 core
const unsigned int SIZE_BITS = 4u; //2^sizebits == chunksize or sqrt(chunkSize)

const unsigned int INDEX_BITS = 3u * SIZE_BITS;
const unsigned int TEXTURE_ID_BITS = 12u;
const unsigned int OCCLUSION_BITS = 8u;
const unsigned int INDEX_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;
const unsigned int OCCLUSION_MASK = 0xFF000000u;

const unsigned int UP = 1u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int UP_RIGHT = 2u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int RIGHT = 4u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int DOWN_RIGHT = 8u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int DOWN = 16u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int DOWN_LEFT = 32u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int LEFT = 64u << (INDEX_BITS + TEXTURE_ID_BITS); 
const unsigned int UP_LEFT = 128u << (INDEX_BITS + TEXTURE_ID_BITS); 

layout(std140) uniform VoxelTransform {
	mat4 VPMatrix;
	vec3 normal;
} transform;

layout(location = 0) in vec4 in_vertex;
layout(location = 1) in uint in_cornerIdx;
layout(location = 2) in vec2 in_texCoord;
layout(location = 3) in uint in_data;

out vec3 texCoord;
out float occlusion;

void main(void)
{
	uint index = in_data & INDEX_MASK;
	float z = float((index >> (SIZE_BITS + SIZE_BITS)));
	float y = float((index >> SIZE_BITS) & ((1u << SIZE_BITS) - 1u));
	float x = float(index & ((1u << SIZE_BITS) - 1u));
	gl_Position = transform.VPMatrix * (in_vertex + vec4(x, y, z, 1.0));

	float textureId = float((in_data & TEXTURE_ID_MASK) >> INDEX_BITS);
	texCoord = vec3(in_texCoord, textureId);

	uint isUpRight = min(in_cornerIdx & UP_RIGHT, 1u);
	uint isUpLeft = min(in_cornerIdx & UP_LEFT, 1u);
	uint isDownRight = min(in_cornerIdx & DOWN_RIGHT, 1u);
	uint isDownLeft = min(in_cornerIdx & DOWN_LEFT, 1u);

	//occlusion values
	uint occlusionBits = in_data & OCCLUSION_MASK;

	uint up = min(occlusionBits & UP, 1u);
	uint upRight = min(occlusionBits & UP_RIGHT, 1u);
	uint right = min(occlusionBits & RIGHT, 1u);
	uint downRight = min(occlusionBits & DOWN_RIGHT, 1u);
	uint down = min(occlusionBits & DOWN, 1u);
	uint downLeft = min(occlusionBits & DOWN_LEFT, 1u);
	uint left = min(occlusionBits & LEFT, 1u);
	uint upLeft = min(occlusionBits & UP_LEFT, 1u);

	occlusion = isUpRight * up + isUpRight * upRight + isUpRight * right;
	occlusion += isUpLeft * up + isUpLeft * upLeft + isUpLeft * left;
	occlusion += isDownRight * down + isDownRight * downRight + isDownRight * right;
	occlusion += isDownLeft * down + isDownLeft * downLeft + isDownLeft * left;
	occlusion = (1.0 - (clamp(occlusion, 0.0, 2.0) * 0.3));
}