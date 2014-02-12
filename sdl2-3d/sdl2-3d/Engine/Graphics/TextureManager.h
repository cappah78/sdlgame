#pragma once

#include <string>
#include <map>

#include "GL/Texture.h"

typedef unsigned int GLuint;
class Texture;

static const unsigned int INVALID_TEXTURE_ID = 0xFFFFFFFF;

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager& copy) = delete;
	~TextureManager();

	GLuint getDefaultTextureID();
	GLuint getTextureID(const std::string texturename);
	const Texture* getTexture(const std::string texturename);
private:
	Texture m_defaultWhiteTexture;
	std::map<std::string, Texture*> m_textureNameMap;
	std::map<std::string, GLuint> m_textureNameIDMap;
};