#include "D3DVertexBuffer.h"

#include "../../../Game.h"

#include <assert.h>
#include <memory>

D3DVertexBuffer::D3DVertexBuffer(unsigned int bufferBindFlags, unsigned int sizeBytes, const void* data, EDrawMode drawMode)
	: m_bindFlags(bufferBindFlags)
	, m_sizeBytes(sizeBytes)
	, m_drawMode(drawMode)
{
	if (sizeBytes == 0 || !data)
	{
		assert(!data);
		m_isInitialized = false;
		return;
	}

	m_isInitialized = true;
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.BindFlags = bufferBindFlags;
	bufferDesc.ByteWidth = sizeBytes;

	if (drawMode == EDrawMode::DYNAMIC)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	}

	D3D11_SUBRESOURCE_DATA dataResource;
	dataResource.pSysMem = data;
	dataResource.SysMemPitch = 0;
	dataResource.SysMemSlicePitch = 0;

	HRESULT result = Game::graphics.getDevice()->CreateBuffer(&bufferDesc, &dataResource, &m_buffer);
	assert(result);

	if (!result)	//set back to uninitialized if failed to create?
	{
		m_isInitialized = false;
	}
}

D3DVertexBuffer::~D3DVertexBuffer()
{

}

inline void D3DVertexBuffer::bind()
{
}

inline void D3DVertexBuffer::update(const void* data, unsigned int numBytes)
{
}

void D3DVertexBuffer::setVertexAttributeParameters(const VertexAttributes& parameters)
{
	
}

inline void D3DVertexBuffer::map(unsigned int numBytes, const void* data, EBufferAccess accessMode, EBufferAccessSync syncMode)
{

}
inline void D3DVertexBuffer::unmap()
{

}