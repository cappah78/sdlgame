#include "SpriteBatch.h"

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <gl\glew.h>

#include "Texture.h"
#include "TextureRegion.h"
#include "ShaderManager.h"
#include "MathUtils.h"

#define FLOATS_PER_SPRITE 16
#define BUFFER_OFFSET(offset) ((void *)(offset))

#define SPRITEBATCH_VERTEX_SHADER "spritebatch.vert"
#define SPRITEBATCH_FRAGMENT_SHADER "spritebatch.frag"
#define SPRITEBATCH_UNIFORM_MVP "m_umvp"

SpriteBatch::SpriteBatch(int size)
	: m_size(size)
	, m_drawing(false)
	, m_lastTexture(0)
	, m_idx(0)
	, m_drawCalls(0)
	, m_blendEnabled(false)
{
	assert((size * 6) < 65535 && "Cannot draw more than 10922 items");
	m_bufferByteSize = size * FLOATS_PER_SPRITE * sizeof(float);

	m_vertices = new float[size * FLOATS_PER_SPRITE];
	memset(m_vertices, 0, m_bufferByteSize);

	m_indices = new GLushort[size * 6];

	GLushort j = 0;
	for (int i = 0; i < size * 6; i += 6, j += 4) {
        m_indices[i + 0] = (j + 0);
        m_indices[i + 1] = (j + 1);
        m_indices[i + 2] = (j + 2);
        m_indices[i + 3] = (j + 2);
        m_indices[i + 4] = (j + 3);
        m_indices[i + 5] = (j + 0);
    }

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_shader = ShaderManager::createShaderProgram(SPRITEBATCH_VERTEX_SHADER, 0, SPRITEBATCH_FRAGMENT_SHADER);
	glUseProgram(m_shader);

	m_mvpLoc = glGetUniformLocation(m_shader, SPRITEBATCH_UNIFORM_MVP);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 6 * sizeof(GLushort), m_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_bufferByteSize, m_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); //position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float))); //uv
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

SpriteBatch::~SpriteBatch()
{
	delete [] m_vertices;
	delete [] m_indices;
}

void SpriteBatch::begin()
{
	assert(!m_drawing && "Call end() before begin()");

	glUseProgram(m_shader);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	m_drawing = true;
}

void SpriteBatch::end()
{
	assert(m_drawing && "Call begin() before end()");

	flush();
	m_drawing = false;
	glUseProgram(0);
}

void SpriteBatch::setProjectionMatrix(glm::mat4 matrix)
{
	assert(m_drawing && "Must call begin() before setting projection matrix");
	assert(m_drawCalls == 0 && "Cannot change projection matrix after drawing");

	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &matrix[0][0]);
}

void SpriteBatch::draw(Texture& texture, float x, float y)
{
	drawUnrotated(texture, x, y, (float) texture.getWidth(), (float) texture.getHeight(), 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y)
{
	drawUnrotated(r.m_texture, x, y, (float) r.m_texture.getWidth(), (float) r.m_texture.getHeight(), r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h)
{
	drawUnrotated(texture, x, y, w, h, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& region, float x, float y, float w, float h)
{
	drawUnrotated(region.m_texture, x, y, w, h, region.u, region.v, region.u2, region.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(texture, x, y, w, h, w / 2.0f, h / 2.0f, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(r.m_texture, x, y, w, h, w / 2, h / 2, angleDeg, r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(texture, x, y, w, h, rotOriginX, rotOriginY, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(r.m_texture, x, y, w, h, w / 2, h / 2, angleDeg, r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::preDraw(Texture& texture)
{
	assert(m_drawing && "Call begin() before drawing");

	if (m_lastTexture == 0)
		swapTexture(texture);
	if (texture.getTextureID() != m_lastTexture)
	{
		flush();
		swapTexture(texture);
	}
	m_drawCalls++;

	assert(m_drawCalls < m_size && "Amount of draw calls exceeded size");
}

void SpriteBatch::drawUnrotated(Texture& t, float x, float y, float w, float h, float u, float v, float u2, float v2)
{
	preDraw(t);

	float x2 = x + w;
	float y2 = y + h;

	//bottom left
	m_vertices[m_idx++] = x;
	m_vertices[m_idx++] = y;
	m_vertices[m_idx++] = u;
	m_vertices[m_idx++] = v;
	
	//top left
	m_vertices[m_idx++] = x;
	m_vertices[m_idx++] = y2;
	m_vertices[m_idx++] = u;
	m_vertices[m_idx++] = v2;

	//top right
	m_vertices[m_idx++] = x2;
	m_vertices[m_idx++] = y2;
	m_vertices[m_idx++] = u2;
	m_vertices[m_idx++] = v2;

	//bottom right
	m_vertices[m_idx++] = x2;
	m_vertices[m_idx++] = y;
	m_vertices[m_idx++] = u2;
	m_vertices[m_idx++] = v;

	if(m_idx > m_size * FLOATS_PER_SPRITE)
		flush();
}

void SpriteBatch::drawRotated(Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg, float u, float v, float u2, float v2)
{
	preDraw(t);

	float totalOffsetX = x + rotOriginX;
	float totalOffsetY = y + rotOriginY;
	
	float px1 = -rotOriginX;
	float py1 = -rotOriginY;
	float px2 = px1;
	float py2 = h - rotOriginY;
	float px3 = w - rotOriginX;
	float py3 = py2;
	float px4 = px3;
	float py4 = py1;

	//corners
	float x1, y1, x2, y2, x3, y3, x4, y4;

	if (angleDeg != 0)
	{
		float cos = math::cosDeg(angleDeg);
		float sin = math::sinDeg(angleDeg);

		x1 = cos * px1 - sin * py1;
		y1 = sin * px1 + cos * py1;

		x2 = cos * px2 - sin * py2;
		y2 = sin * px2 + cos * py2;

		x3 = cos * px3 - sin * py3;
		y3 = sin * px3 + cos * py3;

		x4 = x1 + (x3 - x2);
		y4 = y3 - (y2 - y1);
	}
	else
	{
		x1 = px1;
		y1 = py1;

		x2 = px2;
		y2 = py2;

		x3 = px3;
		y3 = py3;

		x4 = px4;
		y4 = py4;
	}

	x1 += totalOffsetX;
	y1 += totalOffsetY;
	x2 += totalOffsetX;
	y2 += totalOffsetY;
	x3 += totalOffsetX;
	y3 += totalOffsetY;
	x4 += totalOffsetX;
	y4 += totalOffsetY;

	m_vertices[m_idx++] = x1;
	m_vertices[m_idx++] = y1;
	m_vertices[m_idx++] = u;
	m_vertices[m_idx++] = v;

	m_vertices[m_idx++] = x2;
	m_vertices[m_idx++] = y2;
	m_vertices[m_idx++] = u;
	m_vertices[m_idx++] = v2;

	m_vertices[m_idx++] = x3;
	m_vertices[m_idx++] = y3;
	m_vertices[m_idx++] = u2;
	m_vertices[m_idx++] = v2;

	m_vertices[m_idx++] = x4;
	m_vertices[m_idx++] = y4;
	m_vertices[m_idx++] = u2;
	m_vertices[m_idx++] = v;

	if(m_idx > m_size * FLOATS_PER_SPRITE)
		flush();
}

void SpriteBatch::flush()
{
	assert(m_drawing && "Call begin() before flush()");
	if (m_drawCalls == 0)
		return;

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_idx * sizeof(float), m_vertices);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_drawCalls * 6, GL_UNSIGNED_SHORT, 0);

	m_idx = 0;
	m_drawCalls = 0;
}

void SpriteBatch::swapTexture(Texture& texture)
{
	if (texture.getNumComponents() == 4) //rgba
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_blendEnabled = true;
	}
	else if (m_blendEnabled)
	{
		glDisable(GL_BLEND);
		m_blendEnabled = false;
	}

	m_lastTexture = texture.getTextureID();
	texture.bind();
}