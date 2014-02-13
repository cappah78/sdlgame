#include "GLTextureManager.h"

GLTextureManager::GLTextureManager()
	: m_defaultWhiteTexture("Assets/Textures/defaultwhite.png")
{

}

GLTextureManager::~GLTextureManager()
{
	for (auto it : m_textureNameMap)
	{
		delete it.second;
	}
}

GLuint GLTextureManager::getDefaultTextureID()
{
	return m_defaultWhiteTexture.getTextureID();
}

GLuint GLTextureManager::getTextureID(const std::string texturename)
{
	auto it = m_textureNameIDMap.find(texturename);
	if (it != m_textureNameIDMap.end())
	{
		return it->second;
	}
	else
	{
		//Texture* t = new Texture(texturename.c_str(), true, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
		GLTexture* t = new GLTexture(texturename.c_str());
		if (!t->isLoaded())
		{
			delete t;
			return m_defaultWhiteTexture.getTextureID();
		}
		GLuint textureID = t->getTextureID();
		m_textureNameMap.insert(std::make_pair(texturename, t));
		m_textureNameIDMap.insert(std::make_pair(texturename, textureID));
		return textureID;
	}
}

const GLTexture* GLTextureManager::getTexture(const std::string texturename)
{
	auto it = m_textureNameMap.find(texturename);
	if (it != m_textureNameMap.end())
	{
		return it->second;
	}
	else
	{
		GLTexture* t = new GLTexture(texturename.c_str());
		if (!t->isLoaded())
		{
			delete t;
			return NULL;
		}
		GLuint textureID = t->getTextureID();
		m_textureNameMap.insert(std::make_pair(texturename.c_str(), t));
		m_textureNameIDMap.insert(std::make_pair(texturename.c_str(), textureID));
		return t;
	}
}