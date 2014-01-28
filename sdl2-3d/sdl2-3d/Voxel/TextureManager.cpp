#include "TextureManager.h"

#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/GL/Texture.h"

TextureManager::~TextureManager()
{
	for (Texture* t : m_textures)
	{
		delete t;
	}
}

TextureID TextureManager::registerTexture(const std::string& texturename)
{
	auto it = m_textureNameIDMap.find(texturename);
	if (it != m_textureNameIDMap.end())
	{
		return it->second;
	}
	else
	{
		Texture* t = new Texture(texturename.c_str());
		if (!t->isLoaded())
		{
			delete t;
			return INVALID_TEXTURE_ID;
		}

		m_textures.push_back(t);
		m_textureNameIDMap.insert(std::make_pair(texturename, m_lastTextureID));
		return m_lastTextureID++;
	}
}

const Texture* const TextureManager::getTexture(TextureID textureID) const
{
	return m_textures.at(textureID);
}

TextureID TextureManager::getTextureID(const std::string& texturename)
{
	auto it = m_textureNameIDMap.find(texturename);
	if (it != m_textureNameIDMap.end())
		return it->second;
	 else
		return INVALID_TEXTURE_ID;
}