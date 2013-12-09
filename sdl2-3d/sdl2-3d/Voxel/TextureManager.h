#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

typedef unsigned short TextureID;

#include <string>
#include <map>
#include <assert.h>

class TextureArray;

class TextureManager
{
public:
	TextureManager(unsigned int textureWidth, unsigned int textureHeight)
		: m_lastTextureID(0)
		, m_textureWidth(textureWidth)
		, m_textureHeight(textureHeight)
	{};
	TextureManager(const TextureManager& copy) = delete;
	~TextureManager() {};

	TextureID getTextureID(const std::string& texturename);
	TextureArray* generateTextureArray();
private:
	unsigned int m_textureWidth;
	unsigned int m_textureHeight;

	TextureID m_lastTextureID;
	std::map<const std::string, TextureID> m_textureNameIDMap;
};

#endif //TEXTURE_MANAGER_H_