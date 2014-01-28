#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
	: m_indiceBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_positionBuffer.upload(&QUAD_VERTICES[0], 12 * sizeof(float));
	m_positionBuffer.setAttribPointer(0, GL_FLOAT, 3);

	m_indiceBuffer.upload(&QUAD_INDICES[0], 6 * sizeof(unsigned char));
}

QuadRenderer::~QuadRenderer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void QuadRenderer::drawQuad()
{
	glBindVertexArray(m_vao);
	m_positionBuffer.bind();
	m_indiceBuffer.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}