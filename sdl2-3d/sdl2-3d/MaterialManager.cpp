#include "MaterialManager.h"

MaterialManager::MaterialManager() : m_grass()
{ 
	m_grassTex = new Texture("arrow.png");
	m_grassRegion = new TextureRegion(m_grassTex);

	m_grass.setDiffuse(m_grassRegion);
}

MaterialManager::~MaterialManager()
{
	delete m_grassTex;
	delete m_grassRegion;
}

const Material& MaterialManager::getMaterial(Asset asset)
{
	switch (asset)
	{
	case GRASS:
		return m_grass;
	}

	assert(false);
	return m_grass;
}