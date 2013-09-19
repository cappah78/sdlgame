#include "GBuffer.h"

#include <stdio.h>
#include <iostream>

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.h"
#include "LightManager.h"
#include "Camera.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int SHADOW_MAP_SIZE = 1024;

const int CAMERA_TRANSFORM_BINDING_POINT = 1;
const int LIGHT_DATA_BINDING_POINT = 2;

#define ARRAY_SIZE_IN_ELEMENTS(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

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

GBuffer::GBuffer()
	: numLights(0)
{
	loadShaders();
	setupGBuffer();
	setupUniforms();

	GLuint lightDataIdx = glGetUniformBlockIndex(lightPassPPL, "LightData");
	GLuint cameraTransformIdx = glGetUniformBlockIndex(lightPassPPL, "CameraTransform");
	GLuint cameraTransformIdx2 = glGetUniformBlockIndex(GBufferPPL, "CameraTransform");

	glUniformBlockBinding(lightPassPPL, lightDataIdx, LIGHT_DATA_BINDING_POINT);
	glUniformBlockBinding(lightPassPPL, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(GBufferPPL, cameraTransformIdx2, CAMERA_TRANSFORM_BINDING_POINT);


	glGenBuffers(1, &lightDataUB);
	glBindBuffer(GL_UNIFORM_BUFFER, lightDataUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightData), lightData, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, lightDataUB);

	glGenBuffers(1, &cameraTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, cameraTransformUB);

	std::cout << "block bindings:" << std::endl;
	std::cout << lightDataIdx << std::endl;
	std::cout << cameraTransformIdx << std::endl;
	std::cout << cameraTransformIdx2 << std::endl;
}

GBuffer::~GBuffer() 
{

}

void GBuffer::loadShaders()
{
	GBufferPPL = ShaderManager::createShaderProgram("gbuffer.vert", 0, "gbuffer.frag");
	lightPassPPL = ShaderManager::createShaderProgram("lightpass.vert", "lightpass.geom", "lightpass.frag");
}

void GBuffer::setupUniforms()
{
	glUseProgram(lightPassPPL);
	u_colorBufferLoc = glGetUniformLocation(lightPassPPL, "u_colorBuffer");
	glUniform1i(u_colorBufferLoc, 0);
	u_normalBufferLoc = glGetUniformLocation(lightPassPPL, "u_normalBuffer");
	glUniform1i(u_normalBufferLoc, 1);
	u_depthBufferLoc = glGetUniformLocation(lightPassPPL, "u_depthBuffer");
	glUniform1i(u_depthBufferLoc, 2);
	u_shadowMapArrayLoc = glGetUniformLocation(lightPassPPL, "u_shadowMapArray");
	glUniform1i(u_shadowMapArrayLoc, 3);

	u_numLightsLoc = glGetUniformLocation(lightPassPPL, "u_numLights");

	std::cout << "Uniform locs: " << u_colorBufferLoc << ":" << u_normalBufferLoc << ":" << u_depthBufferLoc << std::endl;
}

void GBuffer::setupGBuffer()
{
	std::cerr << "> Setting up G-buffer..." << std::endl;
	// create color buffer
	glGenTextures(1, &colorGBT);
	glBindTexture(GL_TEXTURE_2D, colorGBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create normal buffer
	glGenTextures(1, &normalGBT);
	glBindTexture(GL_TEXTURE_2D, normalGBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create depth buffer
	glGenTextures(1, &depthGBT);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create framebuffer and setup attachments
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorGBT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalGBT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthGBT, 0);

	// create shadow map texture cube array
	glGenTextures(1, &shadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, MAX_LIGHTS,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	const GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	CHECK_FRAMEBUFFER_STATUS();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::use(const Camera& camera)
{
	cameraTransform.VPMatrix = camera.combinedMatrix;
	cameraTransform.VMatrix = camera.viewMatrix;
	cameraTransform.PMatrix = camera.projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, cameraTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, cameraTransformUB);

	glUseProgram(GBufferPPL);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::drawBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(lightPassPPL);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorGBT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalGBT);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);

	glDrawArraysInstanced(GL_POINTS, 0, 1, MAX_LIGHTS);
}

void GBuffer::updateLights(const Camera& camera, LightManager& lightManager)
{
	std::vector<const Light*> lightObjects = lightManager.getLights();
	numLights = MAX_LIGHTS < lightObjects.size() ? MAX_LIGHTS : lightObjects.size();
	//glm::mat4 projMat = glm::perspective(90.0f, 1.0f, 0.01f, 40.0f);

	float angle = 50;

	for (int i = 0; i < numLights; ++i)
	{
		const Light* light = lightObjects.at(i);

		glm::vec4 lightWorldPos(light->position, light->isEnabled);

		lightData[i].color = glm::vec4(light->color, light->linearAttenuation);
		lightData[i].position = lightWorldPos;
		/*
		glm::mat4 viewMatrix = glm::lookAt(
			light->position,
			glm::vec3(0, 0, 1),
			glm::vec3(0, 1, 0)
		);

		lightData[i].VPMatrix = projMat * viewMatrix;
		*/
	}

	std::cout << "Numlights: " << numLights << std::endl;

	glUseProgram(lightPassPPL);
	glUniform1i(u_numLightsLoc, numLights);

	glBindBuffer(GL_UNIFORM_BUFFER, lightDataUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, numLights * sizeof(LightData), lightData);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, lightDataUB);
}
