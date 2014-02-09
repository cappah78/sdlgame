#pragma once

struct IVertexBufferParameters
{
	enum Format { 
		R32_FLOAT, RG32_FLOAT, RGB32_FLOAT, RGBA32_FLOAT, 
		R32_INT, RG32_INT, RGB32_INT, RGBA32_INT,
		R32_UINT, RG32_UINT, RGB32_UINT, RGBA32_UINT
	};
	Format shaderFormat;
	unsigned int strideBytes;
	unsigned int offsetBytes;
	unsigned int instanceStepRate;
};

class IVertexBuffer
{
public:
	IVertexBuffer() {};
	virtual ~IVertexBuffer() {};

	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;
};