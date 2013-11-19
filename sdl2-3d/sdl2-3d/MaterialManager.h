#ifndef MATERIAL_MANAGER_H_
#define MATERIAL_MANAGER_H_

#include "Material.h"
#include "TextureRegion.h"
#include "Texture.h"

enum Asset {
	GRASS = 1
};

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	const Material& getMaterial(Asset asset);

private:
	//Materials
	Material m_grass;

	//Regions
	TextureRegion* m_grassRegion;

	//Textures
	Texture* m_grassTex;
};

#endif //MATERIAL_MANAGER_H_