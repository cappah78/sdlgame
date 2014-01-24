#include "GBuffer.h"

GBuffer::GBuffer(unsigned int numTextures, unsigned int width, unsigned int height)
	: m_numTextures(numTextures)
	, m_textures(numTextures, 0)
{
	init(width, height);
}
GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(m_numTextures, &m_textures[0]);
	glDeleteTextures(1, &m_depthTexture);
}

bool GBuffer::init(unsigned int width, unsigned int height)
{
	/*
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);

	// Create the depth buffer
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	*/

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_depthTexture);

	if (m_numTextures > 0)
	{
		glGenTextures(m_numTextures, &m_textures[0]);
		for (unsigned int i = 0; i < m_numTextures; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	std::vector<GLenum> drawBuffers(m_numTextures);
	for (unsigned int i = 0; i < m_numTextures; ++i)
	{
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	if (m_numTextures > 0)
		glDrawBuffers(m_numTextures, &drawBuffers[0]);
	else
		glDrawBuffer(GL_NONE);

#ifdef _DEBUG
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
#endif //_DEBUG

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


	return true;
}

void GBuffer::unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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

void GBuffer::bindDepthTexture(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
}


void GBuffer::setReadBuffer(unsigned int textureIdx)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + textureIdx);
}