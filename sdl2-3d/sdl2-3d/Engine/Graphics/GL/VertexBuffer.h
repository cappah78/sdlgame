#pragma once

#include "../Model/IVertexBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include <gl\glew.h>
#include "../../Utils/CheckGLError.h"
#include <assert.h>
#include <memory>

#define NULL 0

class VertexBuffer : public IVertexBuffer, public IIndiceBuffer
{
public:
	VertexBuffer(GLenum bufferType = GL_ARRAY_BUFFER, GLenum drawUsage = GL_STREAM_DRAW)
		:  m_id(0)
		, m_bufferType(bufferType)
		, m_drawUsage(drawUsage)
		, m_isEnabled(false)
		, m_attributeIdx(-1)
	{
		glGenBuffers(1, &m_id);
	};

	virtual ~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	};

	VertexBuffer(const VertexBuffer& copy) = delete;

	/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex, 
		GLboolean normalized = false, GLboolean isIntegerType = false, GLuint stride = 0, 
		GLuint offset = 0)
	{
		CHECK_GL_ERROR();
		m_attributeIdx = attributeIdx;
		m_isEnabled = true;

		bind();		
		CHECK_GL_ERROR();

		if (isIntegerType)
			glVertexAttribIPointer(attributeIdx, valuesPerVertex, type, stride, (const GLvoid*) offset);
		else
			glVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, stride, (const GLvoid*) offset);
		CHECK_GL_ERROR();
		glEnableVertexAttribArray(attributeIdx);
		CHECK_GL_ERROR();
	};

	inline void setEnabled(bool enabled = true)
	{
		assert(m_attributeIdx != -1 && "No vertexattribpointer set");
		bind();

		if (enabled)
			glEnableVertexAttribArray(m_attributeIdx);
		else
			glDisableVertexAttribArray(m_attributeIdx);
	}

	inline void setAttribDivisor(unsigned int divisor)
	{
		bind();
		glVertexAttribDivisor(m_attributeIdx, divisor);
	}

	inline void reserve(unsigned int numBytes)
	{
		bind();
		glBufferData(m_bufferType, numBytes, NULL, m_drawUsage);
	}

	virtual void bind() override
	{
		glBindBuffer(m_bufferType, m_id);
	}

	virtual void update(const void* data, unsigned int numBytes) override
	{
		bind();
		glBufferData(m_bufferType, numBytes, data, m_drawUsage);
	}

	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) override
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

private:

	GLuint m_id;
	int m_attributeIdx;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	bool m_isEnabled;
};

/** Set the array that is used to add data, thus allowing sharing with multiple vertex buffers */
/*
inline void setBackingArray(T* data, int sizeBytes)
{
	if (m_ownsData && m_data != NULL)
		delete[] m_data;
	m_data = data;
	m_ownsData = false;
	m_sizeInElements = sizeBytes / sizeof(T);

	glBindBuffer(m_bufferType, m_id);
	glBufferData(m_bufferType, sizeBytes, NULL, m_drawUsage);
};

inline void clearBackingArray()
{
	if (m_ownsData && m_data != NULL)
		delete[] m_data;
	m_data = NULL;
}; 
*/