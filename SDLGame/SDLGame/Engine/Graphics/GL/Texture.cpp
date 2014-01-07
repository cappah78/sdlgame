#include "Texture.h"

#include <iostream>
#include <stdio.h>

#include "../Pixmap.h"
#include <gl\glew.h>

Texture::Texture(const Pixmap& pixmap, bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
	: m_width(pixmap.m_width)
	, m_height(pixmap.m_height)
	, m_numComponents(pixmap.m_numComponents)
{
	setupGLTexture(pixmap, generateMipMaps, minFilter, magFilter, textureWrapS, textureWrapT);
}

Texture::Texture(const char* const fileName, bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	Pixmap p(fileName);
	m_width = p.m_width;
	m_height = p.m_height;
	m_numComponents = p.m_numComponents;
	setupGLTexture(p, generateMipMaps, minFilter, magFilter, textureWrapS, textureWrapT);
}

Texture::~Texture()
{
	
}

void Texture::dispose()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::bind(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::setupGLTexture(const Pixmap& pixmap, bool generateMipMaps, GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
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
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}