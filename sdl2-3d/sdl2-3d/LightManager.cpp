#include "LightManager.h"

#include <gl\glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <algorithm>
#include <stdio.h>
#include <iostream>

#include "Light.h"
#include "Camera.h"
#include "LightSorter.h"
#include "ShaderManager.h"

#define CHECK_GL_ERROR()												\
{																		\
	GLenum glError;														\
	if ((glError = glGetError()) != GL_NO_ERROR) {						\
		std::cerr << "OpenGL error code in '" << __FILE__ << "' at line " << __LINE__ << ": " << gluErrorString(glError) << std::endl;	\
	}																	\
}



LightManager::LightManager()
{
	// create light uniform buffer
	glGenBuffers(1, &lightUB);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightData), lights, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUB);

	// create shadow rendering VP matrix uniform buffer
	glGenBuffers(1, &lightTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, lightTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightTransform), lightTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, lightTransformUB);

	//CHECK_GL_ERROR();

	createPointLight(glm::vec3(0, 60, 0), glm::vec3(0.8f, 0.1f, 0.1f), 1.0f);
	/*
	createPointLight(glm::vec3(50, 20, 200), glm::vec3(1, 0.2f, 0.2f), 0.01f);
	createPointLight(glm::vec3(200, -5, 150), glm::vec3(0.2, 0.2f, 1.0f), 0.01f);

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
	updateBlock(camera);
}

void LightManager::sortLights(const Camera& camera)
{
	std::sort(lightObjects.begin(), lightObjects.end(), LightSorter(camera));
}

void LightManager::updateBlock(const Camera& camera)
{
	int max = MAX_LIGHTS < lightObjects.size() ? MAX_LIGHTS : lightObjects.size();
	glm::mat4 projMat = glm::perspective(90.0f, 1.0f, 0.01f, 40.0f);

	float angle = 50;

	for (int i = 0; i < max; ++i)
	{
		Light* light = lightObjects.at(i);

		glm::vec4 lightWorldPos(camera.viewMatrix * glm::vec4(light->position, 1));
		lightWorldPos.w = light->isEnabled;

		lights[i].color = glm::vec4(light->color, light->linearAttenuation);
		lights[i].position = lightWorldPos;

		glm::mat4 viewMatrix = glm::lookAt(
			light->position,
			glm::vec3(0, 0, 1),
			glm::vec3(0, 1, 0)
		);

		lightTransform[i].VPMatrix = projMat * viewMatrix;
	}

	
	// update light uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, lightUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, max * sizeof(Light), lights);

	// update shadow rendering VP matrix uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, lightTransformUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, max * sizeof(LightTransform), lightTransform);
}

