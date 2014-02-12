#pragma once

struct IIndiceBufferParameters
{
	enum Format
	{
		UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT
	};

	Format format;
	IIndiceBufferParameters(Format format) : format(format) {};
};

class IIndiceBuffer
{
public:

	IIndiceBuffer() {};
	virtual ~IIndiceBuffer() {};

	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;
};