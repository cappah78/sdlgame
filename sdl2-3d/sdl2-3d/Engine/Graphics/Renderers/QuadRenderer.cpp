#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
	: m_positionBuffer(12)
	, m_texcoordBuffer(8)
	, m_indiceBuffer(6, GL_ELEMENT_ARRAY_BUFFER)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_positionBuffer.add(&QUAD_VERTICES[0], 12);
	m_positionBuffer.setAttribPointer(0, GL_FLOAT, 3);
	m_positionBuffer.update();

	m_texcoordBuffer.add(&QUAD_TEXCOORDS[0], 8);
	m_texcoordBuffer.setAttribPointer(1, GL_FLOAT, 2);
	m_texcoordBuffer.update();

	m_indiceBuffer.add(&QUAD_INDICES[0], 6);
	m_indiceBuffer.update();
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