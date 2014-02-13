#pragma once

#include "../Model/IVertexBuffer.h"
#include "../Model/IIndiceBuffer.h"


class D3DVertexBuffer : public IVertexBuffer, public IIndiceBuffer
{
public:
	D3DVertexBuffer();
	virtual ~D3DVertexBuffer() override;
	D3DVertexBuffer(const D3DVertexBuffer& copy) = delete;

	virtual inline void bind() override;
	virtual inline void update(const void* data, unsigned int numBytes) override;
	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) override;

private:

};