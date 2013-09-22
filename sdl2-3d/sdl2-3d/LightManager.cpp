#include "LightManager.h"

#include <algorithm>

#include "Light.h"
#include "Camera.h"
#include "LightSorter.h"


LightManager::LightManager()
{
	createPointLight(glm::vec3(0, 120, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0005f, 0.70f, 8.0f);
	createPointLight(glm::vec3(0, 120, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0005f, 0.70f, 8.0f);
	createPointLight(glm::vec3(0, 120, 0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0005f, 0.70f, 8.0f);
	createPointLight(glm::vec3(0, 120, 0), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0005f, 0.70f, 8.0f);

	createPointLight(glm::vec3(0, 120, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0005f, 0.70f, 8.0f);
}

LightManager::~LightManager()
{

}

Light* LightManager::createPointLight(glm::vec3& position, glm::vec3& direction, glm::vec3& color, float linearAttenuation, float spotRadius, float spotDropoff)
{
	Light* light = new Light(position, direction, color, linearAttenuation, spotRadius, spotDropoff);
	m_lightObjects.push_back(light);
	return light;
}

void LightManager::update(const Camera& camera)
{
	sortLights(camera);
}

void LightManager::sortLights(const Camera& camera)
{
	std::sort(m_lightObjects.begin(), m_lightObjects.end(), LightSorter(camera));
}

std::vector<Light*>& LightManager::getLights()
{
	return m_lightObjects;
}


