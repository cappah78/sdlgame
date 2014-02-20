#pragma once

#include "../Model/IConstantBuffer.h"
#include <gl\glew.h>
#include "../../Utils/CheckGLError.h"

class GLConstantBuffer : public IConstantBuffer
{
public:
	GLConstantBuffer(GLuint shaderID, GLuint bindingPoint, const char* blockName, GLenum drawUsage = GL_STREAM_DRAW)
		: m_drawUsage(drawUsage)
		, m_bindingPoint(bindingPoint)
	{
		glGenBuffers(1, &m_ubo);
		CHECK_GL_ERROR();
		m_uboIndex = glGetUniformBlockIndex(shaderID, blockName);
		CHECK_GL_ERROR();
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glUniformBlockBinding(shaderID, m_uboIndex, m_bindingPoint);
		CHECK_GL_ERROR();

		glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_ubo);
		CHECK_GL_ERROR();

	};
	virtual ~GLConstantBuffer()
	{
		glDeleteBuffers(1, &m_ubo);
	};
	GLConstantBuffer(const GLConstantBuffer& copy) = delete;

	virtual inline void update(const void* data, unsigned int numBytes)
	{
		bind();
		glBufferData(GL_UNIFORM_BUFFER, numBytes, data, m_drawUsage);
	}

	virtual inline void bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	}

private:
	GLenum m_drawUsage;
	GLuint m_ubo;
	GLuint m_bindingPoint;
	GLuint m_uboIndex;
};