#include "GLVertexBuffer.h"

#include "../../Utils/CheckGLError.h"

#include <assert.h>
#include <memory>

GLVertexBuffer::GLVertexBuffer(unsigned int sizeBytes, const void* data, GLenum bufferType, GLenum drawUsage)
	: m_id(0)
	, m_bufferType(bufferType)
	, m_drawUsage(drawUsage)
	, m_isEnabled(false)
	, m_attributeIdx(-1)
{
	glGenBuffers(1, &m_id);

	if (data != NULL && sizeBytes != 0)
		update(data, sizeBytes);
	else if (sizeBytes != 0)
		reserve(sizeBytes);
}

GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
void GLVertexBuffer::setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex,
	GLboolean normalized, GLboolean isIntegerType, GLuint stride, GLuint offset)
{
	m_attributeIdx = attributeIdx;
	m_isEnabled = true;

	bind();
	if (isIntegerType)
		glVertexAttribIPointer(attributeIdx, valuesPerVertex, type, stride, (const GLvoid*) offset);
	else
		glVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, stride, (const GLvoid*) offset);
	glEnableVertexAttribArray(attributeIdx);
	CHECK_GL_ERROR();
}

void GLVertexBuffer::setEnabled(bool enabled)
{
	assert(m_attributeIdx != -1 && "No vertexattribpointer set");
	bind();

	if (enabled)
		glEnableVertexAttribArray(m_attributeIdx);
	else
		glDisableVertexAttribArray(m_attributeIdx);
}

inline void GLVertexBuffer::setAttribDivisor(unsigned int divisor)
{
	bind();
	glVertexAttribDivisor(m_attributeIdx, divisor);
}

inline void GLVertexBuffer::reserve(unsigned int numBytes)
{
	bind();
	glBufferData(m_bufferType, numBytes, NULL, m_drawUsage);
}

inline void GLVertexBuffer::bind()
{
	glBindBuffer(m_bufferType, m_id);
}

inline void GLVertexBuffer::map(unsigned int numBytes, const void* data, EBufferAccess accessMode, EBufferAccessSync syncMode)
{
	
}

inline void GLVertexBuffer::unmap()
{

}

inline void GLVertexBuffer::update(const void* data, unsigned int numBytes)
{
	bind();
	glBufferData(m_bufferType, numBytes, data, m_drawUsage);
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