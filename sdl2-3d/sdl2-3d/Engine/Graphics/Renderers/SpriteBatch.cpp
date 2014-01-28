#include "SpriteBatch.h"

#include <assert.h>
#include <gl\glew.h>

#include "..\GL\Texture.h"
#include "..\GL\TextureRegion.h"
#include "..\..\Utils\ShaderManager.h"

static const unsigned int FLOATS_PER_SPRITE = 16;
static const char* VERT_SHADER_PATH = "Assets/Shaders/spritebatch.vert";
static const char* FRAG_SHADER_PATH = "Assets/Shaders/spritebatch.frag";
static const char* UNIFORM_MAT_NAME = "u_mvp";

SpriteBatch::SpriteBatch(int size)
	: m_size(size)
	, m_drawing(false)
	, m_lastTexture(0)
	, m_drawCalls(0)
	, m_blendEnabled(false)
	, m_indiceBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
	assert((size * 6) < 65535 && "Cannot draw more than 10922 items"); //(indices will run out)

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	std::vector<unsigned short> indiceData;
	indiceData.reserve(size * 6);
	m_verticeData.reserve(size * FLOATS_PER_SPRITE);

	GLushort j = 0;
	for (int i = 0; i < size * 6; i += 6, j += 4) 
	{
		indiceData.push_back(j + 2);
		indiceData.push_back(j + 1);
		indiceData.push_back(j + 0);
		indiceData.push_back(j + 2);
		indiceData.push_back(j + 0);
		indiceData.push_back(j + 3);
    }
	m_indiceBuffer.upload(&indiceData[0], sizeof(indiceData[0]) * indiceData.size());

	m_verticeBuffer.setAttribPointer(0, GL_FLOAT, 2, GL_FALSE, GL_FALSE, 4 * sizeof(float), 0);	//position
	m_verticeBuffer.setAttribPointer(1, GL_FLOAT, 2, GL_FALSE, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float));	//texcoords

	m_shader = ShaderManager::createShaderProgram(VERT_SHADER_PATH, 0, FRAG_SHADER_PATH);
	glUseProgram(m_shader);

	m_mvpLoc = glGetUniformLocation(m_shader, UNIFORM_MAT_NAME);
}

SpriteBatch::~SpriteBatch()
{
	glDeleteShader(m_shader);
	glDeleteVertexArrays(1, &m_vao);
}

void SpriteBatch::begin()
{
	assert(!m_drawing && "Call end() before begin()");

	glUseProgram(m_shader);

	glBindVertexArray(m_vao);

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

void SpriteBatch::draw(const Texture& texture, float x, float y)
{
	drawUnrotated(texture, x, y, (float) texture.getWidth(), (float) texture.getHeight(), 0, 1, 1, 0);
}

void SpriteBatch::draw(const TextureRegion& r, float x, float y)
{
	drawUnrotated(r.m_texture, x, y, (float) r.m_texture.getWidth(), (float) r.m_texture.getHeight(), r.m_texCoords.x, r.m_texCoords.y, r.m_texCoords.z, r.m_texCoords.w);
}

void SpriteBatch::draw(const Texture& texture, float x, float y, float w, float h)
{
	drawUnrotated(texture, x, y, w, h, 0, 1, 1, 0);
}

void SpriteBatch::draw(const TextureRegion& r, float x, float y, float w, float h)
{
	drawUnrotated(r.m_texture, x, y, w, h, r.m_texCoords.x, r.m_texCoords.y, r.m_texCoords.z, r.m_texCoords.w);
}

void SpriteBatch::draw(const Texture& texture, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(texture, x, y, w, h, w / 2.0f, h / 2.0f, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(const TextureRegion& r, float x, float y, float w, float h, float angleDeg)
{
	drawRotated(r.m_texture, x, y, w, h, w / 2.0f, h / 2.0f, angleDeg, r.m_texCoords.x, r.m_texCoords.y, r.m_texCoords.z, r.m_texCoords.w);
}

void SpriteBatch::draw(const Texture& texture, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(texture, x, y, w, h, rotOriginX, rotOriginY, angleDeg, 0, 1, 1, 0);
}

void SpriteBatch::draw(const TextureRegion& r, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg)
{
	drawRotated(r.m_texture, x, y, w, h, w / 2.0f, h / 2.0f, angleDeg, r.m_texCoords.x, r.m_texCoords.y, r.m_texCoords.z, r.m_texCoords.w);
}

void SpriteBatch::preDraw(const Texture& texture)
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

void SpriteBatch::drawUnrotated(const Texture& t, float x, float y, float w, float h, float u, float v, float u2, float v2)
{
	preDraw(t);

	float x2 = x + w;
	float y2 = y + h;

	//bottom left
	m_verticeData.push_back(x);
	m_verticeData.push_back(y);
	m_verticeData.push_back(u);
	m_verticeData.push_back(v);
	
	//top left
	m_verticeData.push_back(x);
	m_verticeData.push_back(y2);
	m_verticeData.push_back(u);
	m_verticeData.push_back(v2);

	//top right
	m_verticeData.push_back(x2);
	m_verticeData.push_back(y2);
	m_verticeData.push_back(u2);
	m_verticeData.push_back(v2);

	//bottom right
	m_verticeData.push_back(x2);
	m_verticeData.push_back(y);
	m_verticeData.push_back(u2);
	m_verticeData.push_back(v);

	if (m_drawCalls > m_size)
		flush();
}

void SpriteBatch::drawRotated(const Texture& t, float x, float y, float w, float h, float rotOriginX, float rotOriginY, float angleDeg, float u, float v, float u2, float v2)
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

	float cos = glm::cos(glm::radians(angleDeg));
	float sin = glm::sin(glm::radians(angleDeg));
		
	x1 = cos * px1 - sin * py1;
	y1 = sin * px1 + cos * py1;
	x2 = cos * px2 - sin * py2;
	y2 = sin * px2 + cos * py2;
	x3 = cos * px3 - sin * py3;
	y3 = sin * px3 + cos * py3;
	x4 = x1 + (x3 - x2);
	y4 = y3 - (y2 - y1);

	x1 += totalOffsetX;
	y1 += totalOffsetY;
	x2 += totalOffsetX;
	y2 += totalOffsetY;
	x3 += totalOffsetX;
	y3 += totalOffsetY;
	x4 += totalOffsetX;
	y4 += totalOffsetY;

	m_verticeData.push_back(x1);
	m_verticeData.push_back(y1);
	m_verticeData.push_back(u);
	m_verticeData.push_back(v);

	m_verticeData.push_back(x2);
	m_verticeData.push_back(y2);
	m_verticeData.push_back(u);
	m_verticeData.push_back(v2);

	m_verticeData.push_back(x3);
	m_verticeData.push_back(y3);
	m_verticeData.push_back(u2);
	m_verticeData.push_back(v2);

	m_verticeData.push_back(x4);
	m_verticeData.push_back(y4);
	m_verticeData.push_back(u2);
	m_verticeData.push_back(v);

	if (m_drawCalls > m_size)
		flush();
}

void SpriteBatch::flush()
{
	assert(m_drawing && "Call begin() before flush()");
	if (m_drawCalls == 0)
		return;

	glBindVertexArray(m_vao);
	m_indiceBuffer.bind();

	m_verticeBuffer.upload(&m_verticeData[0], sizeof(m_verticeData[0]) * m_verticeData.size());
	glDrawElements(GL_TRIANGLES, m_drawCalls * 6, GL_UNSIGNED_SHORT, 0);
	m_verticeData.clear();

	m_drawCalls = 0;
}

void SpriteBatch::swapTexture(const Texture& texture)
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