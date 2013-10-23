#include "GBuffer.h"

#include <stdio.h>
#include <iostream>

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "ShaderManager.h"
#include "LightManager.h"
#include "Camera.h"

#include "MatrixUtils.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SHADOW_MAP_SIZE = 1024;

const int CAMERA_TRANSFORM_BINDING_POINT = 0;
const int Light_DATA_BINDING_POINT = 1;
const int Light_TRANSFORM_BINDING_POINT = 2;

#define ARRAY_SIZE_IN_ELEMENTS(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<m_sizet>(!(sizeof(a) % sizeof(*(a)))))

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
	: m_numLights(0)
{
	loadShaders();
	setupGBuffer();
	setupUniforms();

	GLuint LightDataIdx = glGetUniformBlockIndex(m_LightPassProgram, "LightData");
	GLuint LightTransformIdx = glGetUniformBlockIndex(m_LightPassProgram, "LightTransform");
	GLuint LightDataIdx2 = glGetUniformBlockIndex(m_shadowMultiProgram, "LightData");
	GLuint LightTransformIdx2 = glGetUniformBlockIndex(m_shadowMultiProgram, "LightTransform");
	GLuint LightTransformIdx3 = glGetUniformBlockIndex(m_shadowSingleProgram, "LightTransform");
	GLuint cameraTransformIdx = glGetUniformBlockIndex(m_LightPassProgram, "CameraTransform");
	GLuint cameraTransformIdx2 = glGetUniformBlockIndex(m_gBufferProgram, "CameraTransform");

	glUniformBlockBinding(m_LightPassProgram, LightDataIdx, Light_DATA_BINDING_POINT);
	glUniformBlockBinding(m_LightPassProgram, LightTransformIdx, Light_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(m_shadowMultiProgram, LightDataIdx2, Light_DATA_BINDING_POINT);
	glUniformBlockBinding(m_shadowMultiProgram, LightTransformIdx2, Light_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(m_LightPassProgram, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(m_gBufferProgram, cameraTransformIdx2, CAMERA_TRANSFORM_BINDING_POINT);

	glGenBuffers(1, &m_LightDataUB);
	glBindBuffer(GL_UNIFORM_BUFFER, m_LightDataUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LightS * sizeof(LightData), m_LightData, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, Light_DATA_BINDING_POINT, m_LightDataUB);

	glGenBuffers(1, &m_LightTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, m_LightTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LightS * sizeof(LightTransform), m_LightTransforms, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, Light_TRANSFORM_BINDING_POINT, m_LightTransformUB);

	glGenBuffers(1, &m_cameraTransformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformUB);
}

GBuffer::~GBuffer() 
{

}

void GBuffer::loadShaders()
{
	m_gBufferProgram = ShaderManager::createShaderProgram("gbuffer.vert", 0, "gbuffer.frag");
	m_LightPassProgram = ShaderManager::createShaderProgram("Lightpass.vert", "Lightpass.geom", "Lightpass.frag");
	m_shadowMultiProgram = ShaderManager::createShaderProgram("shadowmulti.vert", "shadowmulti.geom", 0);
	m_shadowSingleProgram = ShaderManager::createShaderProgram("shadowsingle.vert", 0, 0);
	m_forwardShaderProgram = ShaderManager::createShaderProgram("forwardshader.vert", 0, "forwardshader.frag");
}

void GBuffer::setupUniforms()
{
	glUseProgram(m_LightPassProgram);
	m_colorBufferLoc = glGetUniformLocation(m_LightPassProgram, "u_colorBuffer");
	glUniform1i(m_colorBufferLoc, 0);
	m_normalBufferLoc = glGetUniformLocation(m_LightPassProgram, "u_normalBuffer");
	glUniform1i(m_normalBufferLoc, 1);
	m_depthBufferLoc = glGetUniformLocation(m_LightPassProgram, "u_depthBuffer");
	glUniform1i(m_depthBufferLoc, 2);
	m_shadowMapArrayLoc = glGetUniformLocation(m_LightPassProgram, "u_shadowMapArray");
	glUniform1i(m_shadowMapArrayLoc, 3);
	m_numLightsLoc = glGetUniformLocation(m_LightPassProgram, "u_numLights");
}

void GBuffer::setupGBuffer()
{
	std::cerr << "> Setting up G-buffer..." << std::endl;
	// create color buffer
	glGenTextures(1, &m_colorBufferTex);
	glBindTexture(GL_TEXTURE_2D, m_colorBufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create normal buffer
	glGenTextures(1, &m_normalBufferTex);
	glBindTexture(GL_TEXTURE_2D, m_normalBufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create depth buffer
	glGenTextures(1, &m_depthBufferTex);
	glBindTexture(GL_TEXTURE_2D, m_depthBufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create framebuffer and setup attachments
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalBufferTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferTex, 0);

	const GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	CHECK_FRAMEBUFFER_STATUS();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenTextures(1, &m_shadowDepthTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowDepthTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, MAX_LightS,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// Create FBO to render depth into
	glGenFramebuffers(1, &m_shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	// Attach the depth texture to it
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowDepthTex, 0);
	// Disable color rendering as there are no color attachments
	glDrawBuffer(GL_NONE);
}

void GBuffer::use(const Camera& camera)
{
	m_cameraTransform.VPMatrix = camera.m_combinedMatrix;
	m_cameraTransform.VMatrix = camera.m_viewMatrix;
	m_cameraTransform.PMatrix = camera.m_projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformUB);

	glUseProgram(m_gBufferProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void GBuffer::drawBuffer()
{
	glUseProgram(m_LightPassProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorBufferTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalBufferTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_depthBufferTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowDepthTex);

	glDrawArraysInstanced(GL_POINTS, 0, 1, m_numLights);
}

void GBuffer::updateLights(const Camera& camera, LightManager& LightManager)
{
	/*
	Lights LightObjects = LightManager.getLights();
	m_numLights = MAX_LightS < LightObjects.size() ? MAX_LightS : LightObjects.size();

	glm::mat4 LightProjMat(glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1200.0f));
	
	for (int i = 0; i < m_numLights; ++i) 
	{
		Light* Light = LightObjects.at(i);
		LightData& data = m_LightData[i];

		data.position = glm::vec4(Light->m_position, Light->m_isEnabled ? Light->m_spotRadius : 1.0);
		data.color = glm::vec4(Light->m_color, Light->m_linearAttenuation);
		data.direction = glm::vec4(Light->m_direction, Light->m_spotDropoff);

		glm::mat4 LightViewMat(1);
		LightViewMat = glm::lookAt(Light->m_position, Light->m_position + Light->m_direction, glm::vec3(0, 1, 0));
		//MatrixUtils::setToViewMatrix(LightViewMat, Light->m_position, Light->m_direction, glm::vec3(0, 1, 0));
		m_LightTransforms[i].VPMatrix = LightProjMat * LightViewMat;
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, m_LightDataUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_numLights * sizeof(LightData), m_LightData);
	glBindBufferBase(GL_UNIFORM_BUFFER, Light_DATA_BINDING_POINT, m_LightDataUB);

	glBindBuffer(GL_UNIFORM_BUFFER, m_LightTransformUB);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_numLights * sizeof(LightTransform), m_LightTransforms);
	glBindBufferBase(GL_UNIFORM_BUFFER, Light_TRANSFORM_BINDING_POINT, m_LightTransformUB);

	glUseProgram(m_LightPassProgram);
	glUniform1i(m_numLightsLoc, m_numLights);
	glUseProgram(0);
	*/
}

void GBuffer::setupShadows()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowDepthTex, 0);
	glUseProgram(m_shadowMultiProgram);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
}