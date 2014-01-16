#include "GBuffer.h"

#include "../../../Game.h"

GBuffer::GBuffer(unsigned int numTextures)
	: m_numTextures(numTextures)
	, m_textures(numTextures, 0)
{
	init(Game::graphics.getScreenWidth(), Game::graphics.getScreenHeight());
}
GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(m_numTextures, &m_textures[0]);
	glDeleteTextures(1, &m_depthTexture);
}

bool GBuffer::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glGenTextures(m_numTextures, &m_textures[0]);
	glGenTextures(1, &m_depthTexture);

	for (unsigned int i = 0; i < m_numTextures; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	std::vector<GLenum> drawBuffers(m_numTextures);
	for (unsigned int i = 0; i < m_numTextures; ++i)
	{
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(m_numTextures, &drawBuffers[0]);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}
void GBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}
void GBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	for (unsigned int i = 0; i < m_numTextures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
	}
}

void GBuffer::setReadBuffer(unsigned int textureIdx)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + textureIdx);
}