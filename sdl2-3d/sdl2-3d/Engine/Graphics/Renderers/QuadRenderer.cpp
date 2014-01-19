#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_positionBuffer);
	glGenBuffers(1, &m_indiceBuffer);


	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indiceBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
}

QuadRenderer::~QuadRenderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_positionBuffer);
	glDeleteBuffers(1, &m_indiceBuffer);
}

void QuadRenderer::drawQuad()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}