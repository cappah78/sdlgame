#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "TextureRegion.h"

class Material
{
	friend class VoxelBatch;
public:
	Material();
	Material(const TextureRegion* const diffuse);
	Material(const Material& copyMe);
	~Material();

	enum MaterialFlag
	{
		DIFFUSE = 1, NORMAL = 2, SPECULAR = 4, OCCLUSION = 8
	};

	bool hasDiffuse() { return (m_flags & DIFFUSE) != 0; };
	bool hasNormal() { return (m_flags & NORMAL) != 0; };
	bool hasSpecular() { return (m_flags & SPECULAR) != 0; };
	bool hasOcclusion() { return (m_flags & OCCLUSION) != 0; };

	const TextureRegion* const getDiffuse() const { return m_diffuse; };
	const TextureRegion* const getNormal() const { return m_normal; };
	const TextureRegion* const getSpecular() const { return m_specular; };
	const TextureRegion* const getOcclusion() const { return m_occlusion; };

	void setDiffuse(const TextureRegion* const diffuse);
	void setNormal(const TextureRegion* const normal);
	void setSpecular(const TextureRegion* const specular);
	void setOcclusion(const TextureRegion* const occlusion);

private:
	unsigned char m_flags;

	const TextureRegion* m_diffuse;
	const TextureRegion* m_normal;
	const TextureRegion* m_specular;
	const TextureRegion* m_occlusion;
};

#endif //MATERIAL_H_