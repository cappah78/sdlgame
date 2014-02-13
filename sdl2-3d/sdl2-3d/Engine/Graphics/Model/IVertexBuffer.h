#pragma once

#include <vector>

#include "EDrawMode.h"

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

	VertexAttribute(unsigned int idx, const char* name, Format format, unsigned int numElements, bool normalize = false) : 
		m_attributeIndex(idx), 
		m_attributeName(name),
		m_format(format),
		m_numElements(numElements),
		m_normalize(normalize)
	{}
};

struct VertexAttributes
{
	std::vector<VertexAttribute> m_attributes;
	unsigned int m_vertexSize;
	unsigned int m_instanceStepRate;
	//TODO: .cpp
	VertexAttributes(const VertexAttribute* attribute, unsigned int numAttributes, unsigned int instanceStepRate = 0)
	{
		m_vertexSize = 0;
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

struct IVertexBufferParameters
{
	unsigned int m_sizeInBytes;
	const void* m_data;

	bool m_isDynamic;

	EDrawMode m_drawMode;

	IVertexBufferParameters(unsigned int sizeInBytes = 0, const void* data = NULL, EDrawMode drawMode = STATIC, bool isDynamic = true) 
		: m_sizeInBytes(sizeInBytes), m_data(data), m_drawMode(drawMode), m_isDynamic(isDynamic)
	{};
};

class IVertexBuffer
{
public:
	IVertexBuffer() {};
	virtual ~IVertexBuffer() {};

	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) = 0;
	virtual void update(const void* data, unsigned int numBytes) = 0;
	virtual void bind() = 0;
};