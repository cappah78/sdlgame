#include "GBuffer.h"

#include <stdio.h>
#include <iostream>

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include "ShaderManager.h"
#include "LightManager.h"
#include "Camera.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int SHADOW_MAP_SIZE = 1024;

const int CAMERA_TRANSFORM_BINDING_POINT = 0;
const int LIGHT_DATA_BINDING_POINT = 1;
const int LIGHT_TRANSFORM_BINDING_POINT = 2;

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
	GLuint lightTransformIdx = glGetUniformBlockIndex(lightPassPPL, "LightTransform");
	GLuint cameraTransformIdx = glGetUniformBlockIndex(lightPassPPL, "CameraTransform");
	GLuint cameraTransformIdx2 = glGetUniformBlockIndex(GBufferPPL, "CameraTransform");

	glUniformBlockBinding(lightPassPPL, lightDataIdx, LIGHT_DATA_BINDING_POINT);
	glUniformBlockBinding(lightPassPPL, lightTransformIdx, LIGHT_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(lightPassPPL, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(GBufferPPL, cameraTransformIdx2, CAMERA_TRANSFORM_BINDING_POINT);


	glGenBuffers(1, &lightDataUB);
	glBindBuffer(GL_UNIFORM_BUFFER, lightDataUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightData), lightData, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, lightDataUB);

	glGenBuffers(1, &lightTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, lightTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightTransform), lightTransforms, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_TRANSFORM_BINDING_POINT, lightTransformUB);

	glGenBuffers(1, &cameraTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, cameraTransformUB);

	std::cout << "block bindings:" << std::endl;
	std::cout << lightDataIdx << std::endl;
	std::cout << lightTransformIdx << std::endl;
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
	shadowMultiPPL = ShaderManager::createShaderProgram("shadowmulti.vert", "shadowmulti.geom", 0);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create depth buffer
	glGenTextures(1, &depthGBT);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
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

	const GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	CHECK_FRAMEBUFFER_STATUS();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// create shadow map texture cube array
	glGenTextures(1, &shadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, MAX_LIGHTS,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// create shadow rendering framebuffer
	glGenFramebuffers(1, &shadowFB);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
	glDrawBuffer(GL_NONE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowArrayTex, 0);
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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void GBuffer::drawBuffer()
{
	glUseProgram(lightPassPPL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorGBT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalGBT);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);

	glDrawArraysInstanced(GL_POINTS, 0, 1, numLights);
}

void GBuffer::updateLights(const Camera& camera, LightManager& lightManager)
{
	std::vector<const Light*> lightObjects = lightManager.getLights();
	numLights = MAX_LIGHTS < lightObjects.size() ? MAX_LIGHTS : lightObjects.size();

	glm::mat4 lightProjMat(glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 300.0f));

	for (int i = 0; i < numLights; ++i) 
	{
		const Light* light = lightObjects.at(i);
		LightData& data = lightData[i];
		data.position = glm::vec4(light->position, light->isEnabled ? light->spotRadius : 1.0);
		data.color = glm::vec4(light->color, light->linearAttenuation);
		data.direction = glm::vec4(light->direction, light->spotDropoff);

		glm::mat4 lightViewMat = glm::lookAt(light->position, light->position + light->direction, glm::vec3(0, 1, 0));
		lightTransforms[i].VPMatrix = lightProjMat * lightViewMat;
	}

	std::cout << camera.position.x << ":" << camera.position.y << ":" << camera.position.z << std::endl;

	glUseProgram(lightPassPPL);
	glUniform1i(u_numLightsLoc, numLights);

	glBindBuffer(GL_UNIFORM_BUFFER, lightDataUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, numLights * sizeof(LightData), lightData);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, lightDataUB);

	glBindBuffer(GL_UNIFORM_BUFFER, lightTransformUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, numLights * sizeof(LightTransform), lightTransforms);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_TRANSFORM_BINDING_POINT, lightTransformUB);
}

void GBuffer::setupShadows()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowArrayTex, 0);
	glUseProgram(shadowMultiPPL);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
}