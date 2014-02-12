#include "StateBuffer.h"

#include "../Model/IVertexBuffer.h"
#include "../Model/IConstantBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include <GL/glew.h>
#include <assert.h>

bool StateBuffer::s_isEnabled = false;

//TODO: perhaps remove unbinding after every operation for performance

StateBuffer::StateBuffer()
	: m_isEnabled(false)
{
	glGenVertexArrays(1, &m_vao);
}

StateBuffer::~StateBuffer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void StateBuffer::setIndiceBuffer(std::unique_ptr<IIndiceBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void StateBuffer::addVertexBuffer(std::unique_ptr<IVertexBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void StateBuffer::addConstantBuffer(std::unique_ptr<IConstantBuffer>& buffer)
{
	assert(m_isEnabled);
	buffer->bind();
}

void StateBuffer::enable()
{
	assert(!m_isEnabled);
	assert(!s_isEnabled);
	m_isEnabled = true;
	s_isEnabled = true;
	glBindVertexArray(m_vao);
}

void StateBuffer::disable()
{
	assert(m_isEnabled);
	assert(s_isEnabled);
	m_isEnabled = false;
	s_isEnabled = false;
	glBindVertexArray(0);
}