#include "StateBuffer.h"

#include "../Model/IVertexBuffer.h"
#include "../Model/IConstantBuffer.h"

#include <GL/glew.h>

StateBuffer::StateBuffer()
{
	glGenVertexArrays(1, &m_vao);
}

StateBuffer::~StateBuffer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void StateBuffer::addVertexBuffer(IVertexBuffer& buffer)
{
	glBindVertexArray(m_vao);
	buffer.bind();
	glBindVertexArray(0);
}

void StateBuffer::addConstantBuffer(IConstantBuffer& buffer)
{
	glBindVertexArray(m_vao);
	buffer.bind();
	glBindVertexArray(0);
}

void StateBuffer::enable()
{
	glBindVertexArray(m_vao);
}

void StateBuffer::disable()
{
	glBindVertexArray(0);
}