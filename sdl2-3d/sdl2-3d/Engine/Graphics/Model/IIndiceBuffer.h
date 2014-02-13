#pragma once

#include "EDrawMode.h"

struct IIndiceBufferParameters
{
	enum Format
	{
		UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT
	};

	Format m_format;
	EDrawMode m_drawMode;

	unsigned int m_sizeInBytes;
	const void* m_data;

	IIndiceBufferParameters(Format format, unsigned int sizeInBytes = 0, const void* data = NULL, EDrawMode drawMode = STATIC)
		: m_sizeInBytes(sizeInBytes), m_data(data), m_format(format), m_drawMode(drawMode) 
	{};
};

class IIndiceBuffer
{
public:

	IIndiceBuffer() {};
	virtual ~IIndiceBuffer() {};

	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;
};