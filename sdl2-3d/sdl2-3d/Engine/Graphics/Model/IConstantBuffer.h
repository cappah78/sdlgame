#pragma once

struct IConstantBufferParameters
{
	unsigned int m_sizeInBytes;
	const void* m_data;

	IConstantBufferParameters(unsigned int sizeInBytes = 0, const void* data = NULL)
		: m_sizeInBytes(sizeInBytes), m_data(data)
	{};
};

class IConstantBuffer
{
public:
	IConstantBuffer() {};
	virtual ~IConstantBuffer() {};

	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;

private:
};