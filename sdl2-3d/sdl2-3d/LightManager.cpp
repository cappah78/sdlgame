#include "LightManager.h"

#include <algorithm>

#include "Light.h"
#include "Camera.h"
#include "LightSorter.h"


LightManager::LightManager()
{
	createPointLight(glm::vec3(0, 50, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.0001f);
	
	createPointLight(glm::vec3(200, 50, 0), glm::vec3(1.0f, 0.0f, 0.0f), 0.0001f);

	createPointLight(glm::vec3(0, 50, 200), glm::vec3(0.0, 0.0f, 1.0f), 0.0001f);
	/*
	createPointLight(glm::vec3(0, 20, -500), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, -400), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, -300), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, -200), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, -100), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, -000), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, 100), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, 200), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, 300), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, 400), glm::vec3(1, 1, 1), 0.02f);
	createPointLight(glm::vec3(0, 20, 500), glm::vec3(1, 1, 1), 0.02f);
	*/

}

LightManager::~LightManager()
{

}

Light* LightManager::createPointLight(glm::vec3& position, glm::vec3& color, float linearAttenuation)
{
	Light* light = new Light(position, color, linearAttenuation);
	lightObjects.push_back(light);
	return light;
}

void LightManager::update(const Camera& camera)
{
	sortLights(camera);
}

void LightManager::sortLights(const Camera& camera)
{
	std::sort(lightObjects.begin(), lightObjects.end(), LightSorter(camera));
}

std::vector<const Light*>& LightManager::getLights()
{
	return lightObjects;
}


