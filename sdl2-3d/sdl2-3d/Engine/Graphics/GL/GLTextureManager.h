#pragma once

#include <string>
#include <map>

#include "GLTexture.h"

typedef unsigned int GLuint;

static const unsigned int INVALID_TEXTURE_ID = 0xFFFFFFFF;

class GLTextureManager
{
public:
	GLTextureManager();
	GLTextureManager(const GLTextureManager& copy) = delete;
	~GLTextureManager();

	GLuint getDefaultTextureID();
	GLuint getTextureID(const std::string texturename);
	const GLTexture* getTexture(const std::string texturename);
private:

	GLTexture m_defaultWhiteTexture;
	std::map<std::string, GLTexture*> m_textureNameMap;
	std::map<std::string, GLuint> m_textureNameIDMap;
};