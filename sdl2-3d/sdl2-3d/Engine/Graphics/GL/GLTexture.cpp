#include "GLTexture.h"

#include <iostream>
#include <stdio.h>

#include "../Pixmap.h"
#include <gl\glew.h>

GLTexture::GLTexture(const Pixmap& pixmap, bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
	: m_width(pixmap.m_width)
	, m_height(pixmap.m_height)
	, m_numComponents(pixmap.m_numComponents)
	, m_textureID(0)
{
	setupGLTexture(pixmap, generateMipMaps, minFilter, magFilter, textureWrapS, textureWrapT);
}

GLTexture::GLTexture(const char* const fileName, bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
	: m_textureID(0)
{
	Pixmap p(fileName);
	m_width = p.m_width;
	m_height = p.m_height;
	m_numComponents = p.m_numComponents;
	setupGLTexture(p, generateMipMaps, minFilter, magFilter, textureWrapS, textureWrapT);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_textureID);
}

void GLTexture::bind(IShader& shader, unsigned int index)
{
	bind(index);
}

void GLTexture::unbind(IShader& shader, unsigned int index)
{
	unbind(index);
}

inline void GLTexture::bind(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

inline void GLTexture::unbind(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::setupGLTexture(const Pixmap& pixmap, bool generateMipMaps, GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	if (!pixmap.m_data)
	{
		return;
	}

	GLint internalFormat;
	GLenum format = GL_RGB;
	switch (pixmap.m_numComponents)
	{
	case 1: internalFormat = GL_R;
		break;
	case 2: internalFormat = GL_RG;
		break;
	case 3: internalFormat = GL_RGB;
		break;
	case 4: internalFormat = GL_RGBA;
		format = GL_RGBA;
		break;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (generateMipMaps)
	{
		glTexStorage2D(GL_TEXTURE_2D, 3, internalFormat, m_width, m_height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, pixmap.m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixmap.m_data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}