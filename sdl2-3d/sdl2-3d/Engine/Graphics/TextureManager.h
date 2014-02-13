#pragma once

#include <string>
#include <map>

#include "GL/GLTexture.h"

typedef unsigned int GLuint;

static const unsigned int INVALID_TEXTURE_ID = 0xFFFFFFFF;

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager& copy) = delete;
	~TextureManager();

	GLuint getDefaultTextureID();
	GLuint getTextureID(const std::string texturename);
	const GLTexture* getTexture(const std::string texturename);
private:
	GLTexture m_defaultWhiteTexture;
	std::map<std::string, GLTexture*> m_textureNameMap;
	std::map<std::string, GLuint> m_textureNameIDMap;
};