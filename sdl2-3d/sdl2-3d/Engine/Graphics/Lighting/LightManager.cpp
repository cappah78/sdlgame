#include "LightManager.h"

#include <algorithm>

#include "..\Camera.h"
#include "LightSorter.h"

PointLight* p1;
SpotLight* s1;
DirectionalLight* d1;
AmbientLight* a1;

LightManager::LightManager()
{
	p1 = addLight(new PointLight(glm::vec3(-500, 150, 100), glm::vec3(0.0f, 0.3f, 3.0f), 200.0f));
	s1 = addLight(new SpotLight(glm::vec3(0, 500, 0), glm::vec3(0.0f, -1.0f, 0), glm::vec3(3.0f, 0.6f, 0.6f), 800.0f, 0.8f, 7.0f));
	d1 = addLight(new DirectionalLight(glm::vec3(-0.4f, -0.3f, -0.3f), glm::vec3(1.0f, 0.6f, 0.6f), 2.0f));
	a1 = addLight(new AmbientLight(glm::vec3(0.1, 0.1, 0.1)));
}

LightManager::~LightManager()
{
	removeLight(p1);
	removeLight(s1);
	removeLight(d1);
	removeLight(a1);
	delete p1;
	delete s1;
	delete d1;
	delete a1;
}


void LightManager::update(const Camera& camera)
{
	sortLights(camera);
}

void LightManager::sortLights(const Camera& camera)
{
	std::sort(m_pointLights.begin(), m_pointLights.end(), PointLightSorter(camera));
	std::sort(m_spotLights.begin(), m_spotLights.end(), SpotLightSorter(camera));
}

PointLight* LightManager::addLight(PointLight* pointLight)
{
	m_pointLights.push_back(pointLight);
	return pointLight;
}

void LightManager::removeLight(PointLight* pointLight)
{
	m_pointLights.erase(std::remove(m_pointLights.begin(), m_pointLights.end(), pointLight), m_pointLights.end());
}

PointLights& LightManager::getPointLights()
{
	return m_pointLights;
}

SpotLight* LightManager::addLight(SpotLight* spotLight)
{
	m_spotLights.push_back(spotLight);
	return spotLight;
}

void LightManager::removeLight(SpotLight* spotLight)
{
	m_spotLights.erase(std::remove(m_spotLights.begin(), m_spotLights.end(), spotLight), m_spotLights.end());
}

SpotLights& LightManager::getSpotLights()
{
	return m_spotLights;
}

DirectionalLight* LightManager::addLight(DirectionalLight* directionalLight)
{
	m_directionalLights.push_back(directionalLight);
	return directionalLight;
}

void LightManager::removeLight(DirectionalLight* directionalLight)
{
	m_directionalLights.erase(std::remove(m_directionalLights.begin(), m_directionalLights.end(), directionalLight), m_directionalLights.end());
}

DirectionalLights& LightManager::getDirectionalLights()
{
	return m_directionalLights;
}

AmbientLight* LightManager::addLight(AmbientLight* ambientLight)
{
	m_ambientLights.push_back(ambientLight);
	return ambientLight;
}

void LightManager::removeLight(AmbientLight* ambientLight)
{
	m_ambientLights.erase(std::remove(m_ambientLights.begin(), m_ambientLights.end(), ambientLight), m_ambientLights.end());
}

AmbientLights& LightManager::getAmbientLights()
{
	return m_ambientLights;
}

