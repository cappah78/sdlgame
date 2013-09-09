#include "Texture.h"

#include <iostream>
#include <stdio.h>

Texture::Texture(Pixmap& pixmap)
	: width(pixmap.width)
	, height(pixmap.height)
	, numComponents(pixmap.numComponents)
{
	setupGLTexture(pixmap);
}

Texture::Texture(Pixmap& pixmap, GLuint& textureID_)
	: width(pixmap.width)
	, height(pixmap.height)
	, numComponents(pixmap.numComponents)
{
	setupGLTexture(pixmap);
	textureID_ = textureID;
}

Texture::Texture(const char* fileName)
{
	Pixmap p(fileName);
	width = p.width;
	height = p.height;
	numComponents = p.numComponents;
	setupGLTexture(p);
}

Texture::Texture(const char* fileName, GLuint& textureID_)
{
	Pixmap p(fileName);
	width = p.width;
	height = p.height;
	numComponents = p.numComponents;
	setupGLTexture(p);
	textureID_ = textureID;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::setupGLTexture(Pixmap& pixmap)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	GLint internalFormat;
	GLenum format = GL_RGB;
	switch (pixmap.numComponents)
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixmap.data);
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}