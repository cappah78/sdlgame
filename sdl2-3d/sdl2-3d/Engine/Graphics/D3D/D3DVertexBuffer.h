#pragma once

#include "../Model/IVertexBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include <d3d11.h>

class D3DVertexBuffer : public IVertexBuffer, public IIndiceBuffer
{
public:
	D3DVertexBuffer(unsigned int bufferBindFlags, unsigned int sizeBytes, const void* data, EDrawMode isDynamic);
	virtual ~D3DVertexBuffer() override;
	D3DVertexBuffer(const D3DVertexBuffer& copy) = delete;

	virtual inline void bind() override;
	virtual inline void resize(unsigned int numBytes, const void* data) override;
	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) override;

private:
	bool m_isInitialized;
	EDrawMode m_drawMode;
	unsigned int m_bindFlags;
	unsigned int m_sizeBytes;
	ID3D11Buffer* m_buffer;
};