#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include <gl\glew.h>

//template implementation in here instead of having to include a .cpp

template <typename T>
class VertexBuffer
{
public:
	VertexBuffer(unsigned int sizeInElements, GLenum bufferType = GL_ARRAY_BUFFER, GLenum drawUsage = GL_STREAM_DRAW)
		: m_counter(0)
		, m_lastUpdatePos(0)
		, m_ownsData(sizeInElements > 0)
		, m_sizeInElements(sizeInElements)
		, m_id(0)
		, m_bufferType(bufferType)
		, m_drawUsage(drawUsage)
	{
		m_data = m_ownsData ? new T[sizeInElements] : NULL;

		glGenBuffers(1, &m_id);
		glBindBuffer(bufferType, m_id);
		glBufferData(bufferType, m_sizeInElements * sizeof(T), NULL, drawUsage);
	};

	~VertexBuffer()
	{
		if (m_ownsData && m_data != NULL)
			delete[] m_data;
		glDeleteBuffers(1, &m_id);
	};

	VertexBuffer(const VertexBuffer& copy) = delete;
	
	/** Set and enable glVertexAttrib(I)Pointer with the given properties, binds the buffer */
	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex, GLboolean normalized = false, GLboolean isIntegerType = false)
	{
		glBindBuffer(m_bufferType, m_id);
		if (isIntegerType)
			glVertexAttribIPointer(attributeIdx, valuesPerVertex, type, 0, 0);
		else
			glVertexAttribPointer(attributeIdx, valuesPerVertex, type, normalized, 0, 0);

		glEnableVertexAttribArray(attributeIdx);
	};

	inline void bind()
	{
		glBindBuffer(m_bufferType, m_id);
	};

	/** Copy size amount of bytes from data to the buffer */
	inline void add(void* data, unsigned int size)
	{
		assert(m_sizeInElements > m_counter + size);
		memcpy(&m_data[m_counter], data, size);
		m_counter += size;
	};

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
		glBufferSubData(m_bufferType, m_lastUpdatePos, m_counter * sizeof(T), m_data);
		m_lastUpdatePos = m_counter;
	};

	inline void reset()
	{
		m_counter = 0;
		m_lastUpdatePos = 0;
	};

	/** Set the array that is used to add data, thus allowing sharing with multiple vertex buffers */
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

	inline GLuint getID() { return m_id; };
	inline T* getBackingArray() { return m_data; };
	inline unsigned int getSizeInElements() { return m_sizeInElements; };

private:

	unsigned int m_lastUpdatePos;
	unsigned int m_counter;
	unsigned int m_sizeInElements;
	T* m_data;
	GLuint m_id;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	bool m_ownsData;
};

#endif //VERTEX_BUFFER_H_
