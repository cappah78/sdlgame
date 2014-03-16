#include "GLVertexBuffer.h"

#include "../../Utils/CheckGLError.h"

#include <assert.h>
#include <memory>

#include "GLCommandQueue.h"

GLVertexBuffer::GLVertexBuffer(unsigned int sizeBytes, const void* data, GLenum bufferType, GLenum drawUsage)
	: m_id(0)
	, m_bufferType(bufferType)
	, m_drawUsage(drawUsage)
	, m_isEnabled(false)
	, m_isInitialized(false)
	, m_attributeIdx(-1)
{
	glqGenBuffers(1, &m_id);

	if (sizeBytes != 0)
		resize(sizeBytes, data);
}

GLVertexBuffer::~GLVertexBuffer()
{
	glqDeleteBuffers(1, &m_id);
}

/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
void GLVertexBuffer::setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex,
	GLboolean normalized, GLboolean isIntegerType, GLuint stride, GLuint offset)
{
	m_attributeIdx = attributeIdx;
	m_isEnabled = true;

	bind();
	if (isIntegerType)
		glqVertexAttribIPointer(attributeIdx, valuesPerVertex, type, stride, (const GLvoid*) offset);
	else
		glqVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, stride, (const GLvoid*) offset);
	glqEnableVertexAttribArray(attributeIdx);
	glqCheckGLError();
	//CHECK_GL_ERROR();
}

void GLVertexBuffer::setEnabled(bool enabled)
{
	assert(m_attributeIdx != -1 && "No vertexattribpointer set");
	bind();

	if (enabled)
		glqEnableVertexAttribArray(m_attributeIdx);
	else
		glqDisableVertexAttribArray(m_attributeIdx);
}

inline void GLVertexBuffer::setAttribDivisor(unsigned int divisor)
{
	bind();
	glqVertexAttribDivisor(m_attributeIdx, divisor);
}

inline void GLVertexBuffer::resize(unsigned int numBytes, const void* data)
{
	bind();
	/*
	if (data && m_isInitialized)
		glBufferData(m_bufferType, numBytes, NULL, m_drawUsage);
	m_isInitialized = true;*/
	glqBufferData(m_bufferType, numBytes, data, m_drawUsage);
}

inline void GLVertexBuffer::bind()
{
	glqBindBuffer(m_bufferType, m_id);
}

GLenum mapFlagsToGLBitfield(BufferMapFlags flags)
{
	GLenum bitfield = 0;
	if (flags & EBufferMapFlagBits::WRITE)
		bitfield |= GL_MAP_READ_BIT;
	if (flags & EBufferMapFlagBits::READ)
		bitfield |= GL_MAP_READ_BIT;
	if (flags & EBufferMapFlagBits::ASYNC)
		bitfield |= GL_MAP_UNSYNCHRONIZED_BIT;
	if (flags & EBufferMapFlagBits::INVALIDATE_RANGE)
		bitfield |= GL_MAP_INVALIDATE_RANGE_BIT;
	if (flags & EBufferMapFlagBits::INVALIDATE_BUFFER)
		bitfield |= GL_MAP_INVALIDATE_BUFFER_BIT;
	return bitfield;
}


void GLVertexBuffer::setVertexAttributeParameters(const VertexAttributes& parameters)
{
	unsigned int offset = 0;
	unsigned int stride = parameters.m_vertexSize;

	if (parameters.m_attributes.size() == 1)	//if only one attribute, set tightly packed
		stride = 0;

	for (const VertexAttribute& attrib : parameters.m_attributes)
	{
		GLenum type;
		unsigned int dataSize;
		bool isIntegerType = false;
		switch (attrib.m_format)
		{
		case VertexAttribute::Format::UNSIGNED_BYTE:
			type = GL_UNSIGNED_BYTE;
			dataSize = 1;
			isIntegerType = true;
			break;
		case VertexAttribute::Format::UNSIGNED_INT:
			type = GL_UNSIGNED_INT;
			isIntegerType = true;
			dataSize = 4;
			break;
		case VertexAttribute::Format::INT:
			type = GL_INT;
			isIntegerType = true;
			dataSize = 4;
			break;
		case VertexAttribute::Format::FLOAT:
			type = GL_FLOAT;
			dataSize = 4;
			break;
		default:
			assert(false);
			type = GL_FLOAT;
			dataSize = 4;
			break;
		}
		if (attrib.m_normalize)
			isIntegerType = false;

		dataSize *= attrib.m_numElements;
		setAttribPointer(attrib.m_attributeIndex, type, attrib.m_numElements, attrib.m_normalize, isIntegerType, stride, offset);
		offset += dataSize;
	}
}