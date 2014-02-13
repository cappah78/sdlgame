#include "GLStateBuffer.h"

#include "../Model/IVertexBuffer.h"
#include "../Model/IConstantBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include <GL/glew.h>
#include <assert.h>

bool GLStateBuffer::s_isEnabled = false;

//TODO: perhaps remove unbinding after every operation for performance

GLStateBuffer::GLStateBuffer()
	: m_isEnabled(false)
{
	glGenVertexArrays(1, &m_vao);
}

GLStateBuffer::~GLStateBuffer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void GLStateBuffer::setIndiceBuffer(std::unique_ptr<IIndiceBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void GLStateBuffer::addVertexBuffer(std::unique_ptr<IVertexBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void GLStateBuffer::addConstantBuffer(std::unique_ptr<IConstantBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void GLStateBuffer::enable()
{
	assert(!m_isEnabled);
	assert(!s_isEnabled);
	m_isEnabled = true;
	s_isEnabled = true;
	glBindVertexArray(m_vao);
}

void GLStateBuffer::disable()
{
	assert(m_isEnabled);
	assert(s_isEnabled);
	m_isEnabled = false;
	s_isEnabled = false;
	glBindVertexArray(0);
}