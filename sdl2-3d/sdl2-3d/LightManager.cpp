#include "LightManager.h"

#include <gl\glew.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>

#include "Light.h"
#include "Camera.h"
#include "LightSorter.h"

const char* LightManager::U_LIGHTBLOCK_NAME = "LightBlock";
const int LightManager::LIGHTBLOCK_INDEX = 0;

const int SHADOW_MAP_SIZE = 1024;

#define CHECK_GL_ERROR()												\
{																		\
	GLenum glError;														\
	if ((glError = glGetError()) != GL_NO_ERROR) {						\
		std::cerr << "OpenGL error code in '" << __FILE__ << "' at line " << __LINE__ << ": " << gluErrorString(glError) << std::endl;	\
	}																	\
}

#define CHECK_FRAMEBUFFER_STATUS()                            \
{                                                             \
    GLenum status;                                            \
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);		  \
    switch(status) {                                          \
    case GL_FRAMEBUFFER_COMPLETE:                         	  \
        break;                                                \
    case GL_FRAMEBUFFER_UNSUPPORTED:                    	  \
        /* choose different formats */                        \
        std::cerr << "Unsupported framebuffer format!" << std::endl;	  \
        break;                                                \
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:				  \
    	std::cerr << "Incomplete framebuffer attachment!" << std::endl; \
    	break;												  \
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:		  \
    	std::cerr << "Missing framebuffer attachment!" << std::endl; 	  \
    	break;												  \
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:			  \
    	std::cerr << "Incomplete layer target!" << std::endl;			  \
    	break;												  \
    default:                                                  \
        /* programming error; will fail on all hardware */    \
        std::cerr << "Invalid framebuffer format!" << std::endl;		  \
        exit(0);                                          	  \
    }														  \
}

LightManager::LightManager(GLuint shaderId)
{
	u_lightBlock = glGetUniformBlockIndex(shaderId, LightManager::U_LIGHTBLOCK_NAME);
	glUniformBlockBinding(shaderId, u_lightBlock, LightManager::LIGHTBLOCK_INDEX);

	glGenBuffers(1, &lightUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), NULL, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, LightManager::LIGHTBLOCK_INDEX, lightUniformBuffer, 0, sizeof(LightBlock));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	/*
	glGenTextures(1, &shadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, maxLights,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// create shadow rendering framebuffer
	glGenFramebuffers(1, &shadowFb);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFb);
	glDrawBuffer(GL_NONE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowArrayTex, 0);
	CHECK_FRAMEBUFFER_STATUS();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/
	//CHECK_GL_ERROR();

	createPointLight(glm::vec3(0, 0, 100), glm::vec3(1, 1, 1), 0.02f);
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

}

LightManager::~LightManager()
{

}

Light* LightManager::createPointLight(glm::vec3& position, glm::vec3& color, float linearAttenuation)
{
	Light* light = new Light(position, color, linearAttenuation);
	lights.push_back(light);
	return light;
}

void LightManager::update(const Camera& camera)
{
	sortLights(camera);
	updateBlock(camera);

	lightBlock.eyeDirection = camera.direction;
		
	glBindBuffer(GL_UNIFORM_BUFFER, lightUniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightBlock), &lightBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightManager::sortLights(const Camera& camera)
{
	std::sort(lights.begin(), lights.end(), LightSorter(camera));
}

void LightManager::updateBlock(const Camera& camera)
{
	// min (maxLights, lights.size);
	int max = maxLights < lights.size() ? maxLights : lights.size();

	for (int i = 0; i < max; ++i)
	{
		Light* light = lights.at(i);

		glm::vec4 lightWorldPos(camera.viewMatrix * glm::vec4(light->position, 1));
		lightWorldPos.w = light->isEnabled;

		lightBlock.lights[i].color = glm::vec4(light->color, light->linearAttenuation);
		lightBlock.lights[i].position = lightWorldPos;
	}
}