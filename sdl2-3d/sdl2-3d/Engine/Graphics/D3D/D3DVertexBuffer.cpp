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

inline void D3DVertexBuffer::resize(unsigned int numBytes, const void* data)
{

}

struct D3DFormat
{
	DXGI_FORMAT format;
	unsigned int byteSize;
};

D3DFormat getD3DFormatTheAnnoyingWay(VertexAttribute::Format format, unsigned int numElements, bool normalize)
{
	DXGI_FORMAT d3dformat;
	unsigned int perElementByteSize = 0;
	switch (format)
	{
	case VertexAttribute::Format::UNSIGNED_BYTE:
		perElementByteSize = 1;
		if (normalize)
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R8_SNORM; break;
			case 2: d3dformat = DXGI_FORMAT_R8G8_SNORM; break;
			case 3: d3dformat = DXGI_FORMAT_R8G8B8A8_SNORM; assert(false); break;
			case 4: d3dformat = DXGI_FORMAT_R8G8B8A8_SNORM; break;
			default: d3dformat = DXGI_FORMAT_R8G8B8A8_SNORM; assert(false); break;
			}
		}
		else 
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R8_UNORM; break;
			case 2: d3dformat = DXGI_FORMAT_R8G8_UNORM; break;
			case 3: d3dformat = DXGI_FORMAT_R8G8B8A8_UNORM; assert(false); break;
			case 4: d3dformat = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			default: d3dformat = DXGI_FORMAT_R8G8B8A8_UNORM; assert(false); break;
			}
		}
		break;
	case VertexAttribute::Format::UNSIGNED_INT:
		perElementByteSize = 4;
		if (normalize)
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R32_FLOAT; break;
			case 2: d3dformat = DXGI_FORMAT_R32G32_FLOAT; break;
			case 3: d3dformat = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case 4: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			default: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; assert(false); break;
			}
		}
		else
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R32_UINT; break;
			case 2: d3dformat = DXGI_FORMAT_R32G32_UINT; break;
			case 3: d3dformat = DXGI_FORMAT_R32G32B32_UINT; break;
			case 4: d3dformat = DXGI_FORMAT_R32G32B32A32_UINT; break;
			default: d3dformat = DXGI_FORMAT_R32G32B32A32_UINT; assert(false); break;
			}
		}
		break;
	case VertexAttribute::Format::INT:
		perElementByteSize = 4;
		if (normalize)
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R32_FLOAT; break;
			case 2: d3dformat = DXGI_FORMAT_R32G32_FLOAT; break;
			case 3: d3dformat = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case 4: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			default: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; assert(false); break;
			}
		}
		else
		{
			switch (numElements)
			{
			case 1: d3dformat = DXGI_FORMAT_R32_SINT; break;
			case 2: d3dformat = DXGI_FORMAT_R32G32_SINT; break;
			case 3: d3dformat = DXGI_FORMAT_R32G32B32_SINT; break;
			case 4: d3dformat = DXGI_FORMAT_R32G32B32A32_SINT; break;
			default: d3dformat = DXGI_FORMAT_R32G32B32A32_SINT; assert(false); break;
			}
		}
		break;
	case VertexAttribute::Format::FLOAT:
		perElementByteSize = 4;
		switch (numElements)
		{
		case 1: d3dformat = DXGI_FORMAT_R32_FLOAT; break;
		case 2: d3dformat = DXGI_FORMAT_R32G32_FLOAT; break;
		case 3: d3dformat = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case 4: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		default: d3dformat = DXGI_FORMAT_R32G32B32A32_FLOAT; assert(false); break;
		}
		break;
	default:
		assert(false);
		break;
	}

	D3DFormat ret;
	ret.format = d3dformat;
	ret.byteSize = perElementByteSize * numElements;

	return ret;
}


void D3DVertexBuffer::setVertexAttributeParameters(const VertexAttributes& parameters)
{
	D3D11_INPUT_ELEMENT_DESC* descriptions = new D3D11_INPUT_ELEMENT_DESC[parameters.m_attributes.size()];
	for (unsigned int i = 0; i < parameters.m_attributes.size(); ++i)
	{
		const VertexAttribute& attrib = parameters.m_attributes[0];
		D3D11_INPUT_ELEMENT_DESC& desc = descriptions[i];
		ZeroMemory(&desc, sizeof(desc));
		for (const VertexAttribute& attrib : parameters.m_attributes)
		{
			D3DFormat format = getD3DFormatTheAnnoyingWay(attrib.m_format, attrib.m_numElements, attrib.m_normalize);
			desc.Format = format.format;
			desc.AlignedByteOffset = format.byteSize;
			desc.SemanticName = attrib.m_attributeName;
			desc.InputSlot = attrib.m_attributeIndex;
		}
	}
}