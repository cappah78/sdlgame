#version 330 core

const unsigned int WIDTH_BITS = 4u;
const unsigned int HEIGHT_BITS = 4u;

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
out uint textureId;
out float lightLevel;

void main() 
{
	uint index = chunkIdx[0] & 0x00000FFFu;
	textureId = chunkIdx[0] & 0x00FFF000u;
	lightLevel = float(chunkIdx[0] & 0xFF000000u);

	float z = float((index >> (WIDTH_BITS + HEIGHT_BITS)));
	float y = float((index >> WIDTH_BITS) & ((1u << HEIGHT_BITS) - 1u));
	float x = float(index & ((1u << WIDTH_BITS) - 1u));

	vec4 pos = vec4(x, y, z, 1.0);

	gl_Position = vertexTransform.V1MVPMatrix * pos;
	texCoord = vec2(1.0, 0.0); 
	EmitVertex();

	gl_Position = vertexTransform.V2MVPMatrix * pos;
	texCoord = vec2(0.0, 0.0); 
	EmitVertex();

	gl_Position = vertexTransform.V3MVPMatrix * pos;
	texCoord = vec2(1.0, 1.0);
	EmitVertex();

	gl_Position = vertexTransform.V4MVPMatrix * pos;
	texCoord = vec2(0.0, 1.0); 
	EmitVertex();

	EndPrimitive();
}