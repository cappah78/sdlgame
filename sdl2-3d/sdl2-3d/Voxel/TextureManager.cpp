#include "TextureManager.h"

#include "../Engine/Graphics/GL/TextureArray.h"

TextureID TextureManager::getTextureID(const std::string& texturename)
{
	auto it = m_textureNameIDMap.find(texturename);
	if (it == m_textureNameIDMap.end())
	{	//not contained
		m_textureNameIDMap.insert(std::make_pair(texturename, m_lastTextureID));
		return m_lastTextureID++;
	}
	else
	{
		return it->second;
	}
}

TextureArray* TextureManager::generateTextureArray()
{
	std::vector<const std::string*> imageNames(m_textureNameIDMap.size());

	for (auto it = m_textureNameIDMap.begin(); it != m_textureNameIDMap.end(); ++it)
		imageNames.at(it->second) = &it->first;

	return new TextureArray(imageNames, m_textureWidth, m_textureHeight);
}