#pragma once

struct IConstantBufferParameters
{

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