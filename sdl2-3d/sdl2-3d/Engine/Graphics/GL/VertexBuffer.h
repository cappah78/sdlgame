#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include <gl\glew.h>
#include "../../Utils/CheckGLError.h"
#include <assert.h>
#include <memory>

#define NULL 0

class VertexBuffer
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

	~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	};

	VertexBuffer(const VertexBuffer& copy) = delete;
	
	/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex, 
		GLboolean normalized = false, GLboolean isIntegerType = false, GLuint stride = 0, 
		GLuint offset = 0)
	{
		m_attributeIdx = attributeIdx;
		m_isEnabled = true;

		bind();		
		if (isIntegerType)
			glVertexAttribIPointer(attributeIdx, valuesPerVertex, type, stride, (const GLvoid*) offset);
		else
			glVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, stride, (const GLvoid*) offset);
		glEnableVertexAttribArray(attributeIdx);
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
	inline void bindBufferBase(GLuint blockIndex , GLenum target = GL_UNIFORM_BUFFER)
	{
		glBindBufferBase(target, blockIndex, m_id);
	}
	inline void bind()
	{
		glBindBuffer(m_bufferType, m_id);
	}
	inline void reserve(unsigned int numBytes)
	{
		bind();
		glBufferData(m_bufferType, numBytes, NULL, m_drawUsage);
	}
	inline void upload(void* data, unsigned int numBytes)
	{
		bind();
		glBufferData(m_bufferType, numBytes, data, m_drawUsage);
	}
	/** offset: offset within data array. */
	inline void updateRange(void* data, unsigned int fromBytes, unsigned int numBytes)
	{
		glBufferSubData(m_bufferType, fromBytes, numBytes, data);
	}

private:

	GLuint m_id;
	int m_attributeIdx;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	GLenum m_dataType;
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

#endif //VERTEX_BUFFER_H_
