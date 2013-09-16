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
#define SPRITEBATCH_UNIFORM_MVP "u_mvp"

SpriteBatch::SpriteBatch(int size_)
	: size(size_)
	, drawing(false)
	, lastTexture(0)
	, idx(0)
	, drawCalls(0)
	, blendEnabled(false)
{
	assert((size * 6) < 65535 && "Cannot draw more than 10922 items");
	bufferByteSize = size * FLOATS_PER_SPRITE * sizeof(float);

	vertices = new float[size * FLOATS_PER_SPRITE];
	memset(vertices, 0, bufferByteSize);

	indices = new GLushort[size * 6];

	GLushort j = 0;
	for (int i = 0; i < size * 6; i += 6, j += 4) {
        indices[i + 0] = (j + 0);
        indices[i + 1] = (j + 1);
        indices[i + 2] = (j + 2);
        indices[i + 3] = (j + 2);
        indices[i + 4] = (j + 3);
        indices[i + 5] = (j + 0);
    }

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	shader = ShaderManager::createShaderProgram(SPRITEBATCH_VERTEX_SHADER, 0, SPRITEBATCH_FRAGMENT_SHADER);
	glUseProgram(shader);

	mvp = glGetUniformLocation(shader, SPRITEBATCH_UNIFORM_MVP);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 6 * sizeof(GLushort), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferByteSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); //position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float))); //uv
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

SpriteBatch::~SpriteBatch()
{
	delete [] vertices;
	delete [] indices;
}

void SpriteBatch::begin()
{
	assert(!drawing && "Call end() before begin()");

	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	drawing = true;
}

void SpriteBatch::end()
{
	assert(drawing && "Call begin() before end()");

	flush();
	drawing = false;
	glUseProgram(0);
}

void SpriteBatch::setProjectionMatrix(glm::mat4 matrix)
{
	assert(drawing && "Must call begin() before setting projection matrix");
	assert(drawCalls == 0 && "Cannot change projection matrix after drawing");

	glUniformMatrix4fv(mvp, 1, GL_FALSE, &matrix[0][0]);
}

void SpriteBatch::draw(Texture& texture, float x, float y)
{
	drawUnrotated(texture, x, y, (float) texture.getWidth(), (float) texture.getHeight(), 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y)
{
	drawUnrotated(r.texture, x, y, (float) r.texture.getWidth(), (float) r.texture.getHeight(), r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h)
{
	drawUnrotated(texture, x, y, w, h, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& region, float x, float y, float w, float h)
{
	drawUnrotated(region.texture, x, y, w, h, region.u, region.v, region.u2, region.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(texture, x, y, w, h, w / 2.0f, h / 2.0f, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(r.texture, x, y, w, h, w / 2, h / 2, angleDeg, r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::draw(Texture& texture, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(texture, x, y, w, h, rotOriginX, rotOriginY, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(TextureRegion& r, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(r.texture, x, y, w, h, w / 2, h / 2, angleDeg, r.u, r.v, r.u2, r.v2);
}

void SpriteBatch::preDraw(Texture& texture)
{
	assert(drawing && "Call begin() before drawing");

	if (lastTexture == 0)
		swapTexture(texture);
	if (texture.getTextureID() != lastTexture)
	{
		flush();
		swapTexture(texture);
	}
	drawCalls++;

	assert(drawCalls < size && "Amount of draw calls exceeded size");
}

void SpriteBatch::drawUnrotated(Texture& t, float x, float y, float w, float h, float u, float v, float u2, float v2)
{
	preDraw(t);

	float x2 = x + w;
	float y2 = y + h;

	//bottom left
	vertices[idx++] = x;
	vertices[idx++] = y;
	vertices[idx++] = u;
	vertices[idx++] = v;
	
	//top left
	vertices[idx++] = x;
	vertices[idx++] = y2;
	vertices[idx++] = u;
	vertices[idx++] = v2;

	//top right
	vertices[idx++] = x2;
	vertices[idx++] = y2;
	vertices[idx++] = u2;
	vertices[idx++] = v2;

	//bottom right
	vertices[idx++] = x2;
	vertices[idx++] = y;
	vertices[idx++] = u2;
	vertices[idx++] = v;

	if(idx > size * FLOATS_PER_SPRITE)
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

	vertices[idx++] = x1;
	vertices[idx++] = y1;
	vertices[idx++] = u;
	vertices[idx++] = v;

	vertices[idx++] = x2;
	vertices[idx++] = y2;
	vertices[idx++] = u;
	vertices[idx++] = v2;

	vertices[idx++] = x3;
	vertices[idx++] = y3;
	vertices[idx++] = u2;
	vertices[idx++] = v2;

	vertices[idx++] = x4;
	vertices[idx++] = y4;
	vertices[idx++] = u2;
	vertices[idx++] = v;

	if(idx > size * FLOATS_PER_SPRITE)
		flush();
}

void SpriteBatch::flush()
{
	assert(drawing && "Call begin() before flush()");
	if (drawCalls == 0)
		return;

	glBufferSubData(GL_ARRAY_BUFFER, 0, idx * sizeof(float), vertices);
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, drawCalls * 6, GL_UNSIGNED_SHORT, 0);

	idx = 0;
	drawCalls = 0;
}

void SpriteBatch::swapTexture(Texture& texture)
{
	if (texture.getNumComponents() == 4) //rgba
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		blendEnabled = true;
	}
	else if (blendEnabled)
	{
		glDisable(GL_BLEND);
		blendEnabled = false;
	}

	lastTexture = texture.getTextureID();
	texture.bind();
}