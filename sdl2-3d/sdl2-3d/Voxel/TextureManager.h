#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

typedef unsigned short TextureID;

#include <string>
#include <map>
#include <vector>
#include <assert.h>

class TextureArray;
class Texture;

class TextureManager
{
public:

	static const unsigned short INVALID_TEXTURE_ID = 65535;

	TextureManager()
		: m_lastTextureID(0)
	{};
	TextureManager(const TextureManager& copy) = delete;
	~TextureManager();

	TextureID registerTexture(const std::string& texturename);
	const Texture* const getTexture(TextureID textureID) const;

	TextureID getTextureID(const std::string& texturename);
private:

	TextureID m_lastTextureID;
	std::map<const std::string, TextureID> m_textureNameIDMap;
	std::vector<Texture*> m_textures;
};

#endif //TEXTURE_MANAGER_H_