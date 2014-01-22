#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include <gl\glew.h>
#include "../../Utils/CheckGLError.h"
#include <assert.h>
#include <memory>

#define NULL 0


//template implementation in here instead of having to include a .cpp

template <typename T>
class VertexBuffer
{
public:
	VertexBuffer(unsigned int sizeInElements, GLenum bufferType = GL_ARRAY_BUFFER, GLenum drawUsage = GL_STREAM_DRAW, T* data = NULL)
		: m_counter(0)
		, m_lastUpdatePos(0)
		, m_ownsData(sizeInElements > 0)
		, m_sizeInElements(sizeInElements)
		, m_id(0)
		, m_bufferType(bufferType)
		, m_drawUsage(drawUsage)
		, m_isEnabled(false)
		, m_attributeIdx(-1)
	{
		m_data = data == NULL ? new T[sizeInElements] : data;
		m_ownsData = (m_data == NULL);

		glGenBuffers(1, &m_id);
	};

	~VertexBuffer()
	{
		if (m_ownsData && m_data != NULL)
			delete[] m_data;
		glDeleteBuffers(1, &m_id);
	};

	VertexBuffer(const VertexBuffer& copy) = delete;
	
	/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex, 
		GLboolean normalized = false, GLboolean isIntegerType = false, GLuint stride = 0, 
		GLuint offsetBytes = 0)
	{
		CHECK_GL_ERROR();

		m_attributeIdx = attributeIdx;
		m_isEnabled = true;

		glBindBuffer(m_bufferType, m_id);
		if (isIntegerType)
			glVertexAttribIPointer(attributeIdx, valuesPerVertex, type, stride, 
			(const GLvoid*) offsetBytes);
		else
			glVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, 
			stride, (const GLvoid*) offsetBytes);
		glEnableVertexAttribArray(attributeIdx);
		CHECK_GL_ERROR();
	};

	inline void setEnabled(bool enabled = true)
	{
		assert(m_attributeIdx != -1 && "No vertexattribpointer set");
		glBindBuffer(m_bufferType, m_id);

		if (enabled)
			glEnableVertexAttribArray(m_attributeIdx);
		else
			glDisableVertexAttribArray(m_attributeIdx);
	}

	inline void bindBufferBase(GLuint blockIndex , GLenum target = GL_UNIFORM_BUFFER)
	{
		glBindBufferBase(target, blockIndex, m_id);
	}

	inline void bind()
	{
		glBindBuffer(m_bufferType, m_id);
	};

	inline void add(T* data, unsigned int numElements)
	{
		assert(m_sizeInElements >= m_counter + numElements);
		memcpy((void*)&m_data[m_counter], data, numElements * sizeof(T));
		m_counter += numElements;
	}

	/** Add data to the buffer, copied by value since it assumed that vertex buffer elements are generally very small */
	inline void add(T data)
	{
		assert(m_sizeInElements > m_counter);
		m_data[m_counter++] = data;
	};

	/** Upload the added data to the gpu, binds the buffer */
	inline void update()
	{
		glBindBuffer(m_bufferType, m_id);
		glBufferData(m_bufferType, m_sizeInElements * sizeof(T), m_data, m_drawUsage);
	};

	inline void reset()
	{
		m_counter = 0;
		m_lastUpdatePos = 0;
	};

	inline GLuint getID() { return m_id; };
	inline T* getBackingArray() { return m_data; };
	inline unsigned int getSizeInElements() { return m_counter; };

private:

	unsigned int m_lastUpdatePos;
	unsigned int m_counter;
	unsigned int m_sizeInElements;
	T* m_data;
	GLuint m_id;
	int m_attributeIdx;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	bool m_ownsData;
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
