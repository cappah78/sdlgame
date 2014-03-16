#pragma once

#include "../Model/IVertexBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include "GLTypes.h"
#include "GLDefines.h"

class GLVertexBuffer : public IVertexBuffer, public IIndiceBuffer
{
public:
	GLVertexBuffer(unsigned int sizeBytes = 0, const void* data = NULL, GLenum bufferType = GL_ARRAY_BUFFER, GLenum drawUsage = GL_STATIC_DRAW);
	virtual ~GLVertexBuffer() override;
	GLVertexBuffer(const GLVertexBuffer& copy) = delete;

	virtual inline void bind() override;
	virtual inline void resize(unsigned int numBytes, const void* data = 0) override;
	virtual void setVertexAttributeParameters(const VertexAttributes& parameters) override;

	void setAttribPointer(GLuint attributeIdx, GLenum type, unsigned int valuesPerVertex,
		GLboolean normalized = false, GLboolean isIntegerType = false, GLuint stride = 0,
		GLuint offset = 0);
	void setEnabled(bool enabled = true);

	inline void setAttribDivisor(unsigned int divisor);

private:

	GLuint m_id;
	int m_attributeIdx;
	GLenum m_bufferType;
	GLenum m_drawUsage;
	bool m_isEnabled;
	bool m_isInitialized;
};