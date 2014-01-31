#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <assert.h>

#include "GL/Texture.h"

typedef unsigned int GLuint;

static const unsigned int INVALID_TEXTURE_ID = 0xFFFFFFFF;

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager& copy) = delete;
	~TextureManager();

	GLuint getTextureID(const std::string texturename);
	const Texture* getTexture(const std::string texturename);
private:
	Texture m_defaultWhiteTexture;
	std::map<std::string, Texture*> m_textureNameMap;
	std::map<std::string, GLuint> m_textureNameIDMap;
};

#endif //TEXTURE_MANAGER_H_