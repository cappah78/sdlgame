#include "Texture.h"

#include <iostream>
#include <stdio.h>

#include "Pixmap.h"
#include <gl\glew.h>

Texture::Texture(Pixmap& pixmap)
	: m_width(pixmap.m_width)
	, m_height(pixmap.m_height)
	, m_numComponents(pixmap.m_numComponents)
{
	setupGLTexture(pixmap);
}

Texture::Texture(Pixmap& pixmap, GLuint& textureID)
	: m_width(pixmap.m_width)
	, m_height(pixmap.m_height)
	, m_numComponents(pixmap.m_numComponents)
{
	setupGLTexture(pixmap);
	m_textureID = textureID;
}

Texture::Texture(const char* fileName)
{
	Pixmap p(fileName);
	m_width = p.m_width;
	m_height = p.m_height;
	m_numComponents = p.m_numComponents;
	setupGLTexture(p);
}

Texture::Texture(const char* fileName, GLuint& textureID)
{
	Pixmap p(fileName);
	m_width = p.m_width;
	m_height = p.m_height;
	m_numComponents = p.m_numComponents;
	setupGLTexture(p);
	m_textureID = textureID;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::setupGLTexture(Pixmap& pixmap)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
 
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixmap.m_data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}