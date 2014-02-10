#pragma once

#include <vector>

struct VertexAttribute
{
	enum Format
	{
		UNSIGNED_BYTE, UNSIGNED_INT, INT, FLOAT
	};

	unsigned int m_attributeIndex;
	const char* m_attributeName;
	Format m_format;
	unsigned int m_numElements;
	bool m_normalize;
};

struct IVertexBufferParameters
{
	std::vector<VertexAttribute> m_attributes;
	unsigned int m_vertexSize;
	unsigned int m_instanceStepRate;
	//TODO: .cpp
	IVertexBufferParameters(const VertexAttribute* attribute, unsigned int numAttributes, unsigned int instanceStepRate = 0)
	{
		m_instanceStepRate = instanceStepRate;
		m_attributes.reserve(numAttributes);
		for (unsigned int i = 0; i < numAttributes; ++i)
		{
			m_attributes.push_back(attribute[i]);
			unsigned int dataSize = attribute->m_format == VertexAttribute::Format::UNSIGNED_BYTE ? 1 : 4;
			m_vertexSize += dataSize * attribute->m_numElements;
		}
	}
};

class IVertexBuffer
{
public:
	IVertexBuffer() {};
	virtual ~IVertexBuffer() {};

	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;
};