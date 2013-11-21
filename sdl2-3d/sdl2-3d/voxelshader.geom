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

layout(std140) uniform VertexTransform {
	mat4 V1MVPMatrix;
	mat4 V2MVPMatrix;
	mat4 V3MVPMatrix;
	mat4 V4MVPMatrix;
} vertexTransform;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in uint chunkIdx[1];

out vec2 texCoord;
out float textureId;
out float occlusion;

void main() 
{
	uint index = chunkIdx[0] & INDEX_MASK;
	uint occlusionBits = chunkIdx[0] & OCCLUSION_MASK;
	textureId = float((chunkIdx[0] & TEXTURE_ID_MASK) >> INDEX_BITS);

	float z = float((index >> (SIZE_BITS + SIZE_BITS)));
	float y = float((index >> SIZE_BITS) & ((1u << SIZE_BITS) - 1u));
	float x = float(index & ((1u << SIZE_BITS) - 1u));
	vec4 pos = vec4(x, y, z, 1.0);

	gl_Position = vertexTransform.V1MVPMatrix * pos;
	texCoord = vec2(1.0, 0.0);
	occlusion = min(occlusionBits & UP, 1u);
	occlusion += min(occlusionBits & UP_RIGHT, 1u);
	occlusion += min(occlusionBits & RIGHT, 1u);
	occlusion = clamp(occlusion, 0.0, 2.0);
	EmitVertex(); //topright

	gl_Position = vertexTransform.V2MVPMatrix * pos;
	texCoord = vec2(0.0, 0.0);	
	occlusion = min(occlusionBits & UP, 1u);
	occlusion += min(occlusionBits & UP_LEFT, 1u);
	occlusion += min(occlusionBits & LEFT, 1u);
	occlusion = clamp(occlusion, 0.0, 2.0);
	EmitVertex(); //topleft

	gl_Position = vertexTransform.V3MVPMatrix * pos;
	texCoord = vec2(1.0, 1.0); 
	occlusion = min(occlusionBits & DOWN, 1u);
	occlusion += min(occlusionBits & DOWN_RIGHT, 1u);
	occlusion += min(occlusionBits & RIGHT, 1u);
	occlusion = clamp(occlusion, 0.0, 2.0);
	EmitVertex(); //bottomright

	gl_Position = vertexTransform.V4MVPMatrix * pos;
	texCoord = vec2(0.0, 1.0); 
	occlusion = min(occlusionBits & DOWN, 1u);
	occlusion += min(occlusionBits & DOWN_LEFT, 1u);
	occlusion += min(occlusionBits & LEFT, 1u);
	occlusion = clamp(occlusion, 0.0, 2.0);
	EmitVertex(); //bottomleft

	EndPrimitive();
}