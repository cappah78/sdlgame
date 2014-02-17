#pragma once

#include "../Model/IVertexBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include <gl\glew.h>

class GLVertexBuffer : public IVertexBuffer, public IIndiceBuffer
{
public:
	GLVertexBuffer(unsigned int sizeBytes = 0, const void* data = NULL, GLenum bufferType = GL_ARRAY_BUFFER, GLenum drawUsage = GL_STATIC_DRAW);
	virtual ~GLVertexBuffer() override;
	GLVertexBuffer(const GLVertexBuffer& copy) = delete;

	virtual inline void bind() override;
	virtual inline void update(const void* data, unsigned int numBytes) override;
	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) override;
	virtual inline void map(unsigned int numBytes, const void* data, EBufferAccess accessMode, EBufferAccessSync syncMode) override;
	virtual inline void unmap() override;

	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex,
		GLboolean normalized = false, GLboolean isIntegerType = false, GLuint stride = 0,
		GLuint offset = 0);
	void setEnabled(bool enabled = true);

	inline void setAttribDivisor(unsigned int divisor);
	inline void reserve(unsigned int numBytes);

private:

	GLuint m_id;
	int m_attributeIdx;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	bool m_isEnabled;
};