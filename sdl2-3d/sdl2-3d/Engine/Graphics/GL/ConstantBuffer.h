#pragma once

#include "../Model/IConstantBuffer.h"
#include <gl\glew.h>
#include "Shader.h"

class ConstantBuffer : public IConstantBuffer
{
public:
	ConstantBuffer(GLuint shaderID, GLuint bindingPoint, const char* blockName, GLenum drawUsage = GL_STREAM_DRAW)
		: m_drawUsage(drawUsage)
		, m_bindingPoint(bindingPoint)
	{
		glGenBuffers(1, &m_ubo);

		m_uboIndex = glGetUniformBlockIndex(shaderID, blockName);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glUniformBlockBinding(shaderID, m_uboIndex, m_bindingPoint);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_ubo);
	};
	virtual ~ConstantBuffer()
	{
		glDeleteBuffers(1, &m_ubo);
	};
	ConstantBuffer(const ConstantBuffer& copy) = delete;

	virtual void update(const void* data, unsigned int numBytes)
	{
		bind();
		glBufferData(GL_UNIFORM_BUFFER, numBytes, data, m_drawUsage);
	}

	virtual void bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	}

private:
	GLenum m_drawUsage;
	GLuint m_ubo;
	GLuint m_bindingPoint;
	GLuint m_uboIndex;
};