#include "Material.h"

Material::Material()
{

}

Material::Material(const TextureRegion* const diffuse) : m_diffuse(diffuse)
{
	m_flags |= DIFFUSE;
}

Material::Material(const Material& copyMe)
{

}

Material::~Material()
{

}

void Material::setDiffuse(const TextureRegion* const diffuse)
{
	m_diffuse = diffuse;
}

void Material::setNormal(const TextureRegion* const normal)
{
	m_normal = normal;
}

void Material::setSpecular(const TextureRegion* const specular)
{
	m_specular = specular;
}

void Material::setOcclusion(const TextureRegion* const diffuse)
{
	m_diffuse = diffuse;
}