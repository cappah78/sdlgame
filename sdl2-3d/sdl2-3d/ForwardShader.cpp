#include "ForwardShader.h"

#include "ShaderManager.h"
#include "Light.h"

#include <iostream>
#include <stdio.h>

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const int SHADOW_MAP_SIZE = 1024;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int CAMERA_TRANSFORM_BINDING_POINT = 0;
const int LIGHT_DATA_BINDING_POINT = 1;
const int LIGHT_TRANSFORM_BINDING_POINT = 2;

const float LIGHT_NEAR = 1.0f;
const float LIGHT_FAR = 1200.0f;

ForwardShader::ForwardShader()
	: m_numLights(0)
{
	m_forwardShaderProgram = ShaderManager::createShaderProgram("forwardshader.vert", 0, "forwardshader.frag");
	m_shadowProgram = ShaderManager::createShaderProgram("shadowmulti.vert", "shadowmulti.geom", 0);
	setupUniforms();
	setupBuffers();
	setupShadowFramebuffer();
}

ForwardShader::~ForwardShader()
{

}

void ForwardShader::setupUniforms()
{
	glUseProgram(m_forwardShaderProgram);
	m_numLightsLoc = glGetUniformLocation(m_forwardShaderProgram, "u_numLights");

	GLint m_texLoc = glGetUniformLocation(m_forwardShaderProgram, "tex");
	glUniform1i(m_texLoc, 0);
	GLint m_shadowArrayTexLoc = glGetUniformLocation(m_forwardShaderProgram, "u_shadowMapArray");
	glUniform1i(m_shadowArrayTexLoc, 1);

	GLuint lightDataIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "LightData");
	GLuint lightTransformIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "LightTransform");
	GLuint cameraTransformIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "CameraTransform");

	GLuint lightDataIdx2 = glGetUniformBlockIndex(m_shadowProgram, "LightData");
	GLuint lightTransformIdx2 = glGetUniformBlockIndex(m_shadowProgram, "LightTransform");

	glUniformBlockBinding(m_forwardShaderProgram, lightDataIdx, LIGHT_DATA_BINDING_POINT);
	glUniformBlockBinding(m_forwardShaderProgram, lightTransformIdx, LIGHT_TRANSFORM_BINDING_POINT);
	glUniformBlockBinding(m_forwardShaderProgram, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);

	glUniformBlockBinding(m_shadowProgram, lightDataIdx2, LIGHT_DATA_BINDING_POINT);
	glUniformBlockBinding(m_shadowProgram, lightTransformIdx2, LIGHT_TRANSFORM_BINDING_POINT);
}

void ForwardShader::setupBuffers()
{
	glGenBuffers(1, &m_lightDataBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_lightDataBuffer);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightData), m_lightData, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, m_lightDataBuffer);

	glGenBuffers(1, &m_lightTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_lightTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightTransform), m_lightTransforms, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_TRANSFORM_BINDING_POINT, m_lightTransformBuffer);

	glGenBuffers(1, &m_cameraTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);
}

void ForwardShader::use(const Camera& camera)
{
	m_cameraTransform.VPMatrix = camera.m_combinedMatrix;
	m_cameraTransform.VMatrix = camera.m_viewMatrix;
	m_cameraTransform.PMatrix = camera.m_projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);

	glUseProgram(m_forwardShaderProgram);
	glUniform1i(m_numLightsLoc, m_numLights);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowArrayTex);
	glActiveTexture(GL_TEXTURE0);
}

void ForwardShader::updateLights(const Camera& camera, LightManager& lightManager)
{
	std::vector<Light*> lightObjects = lightManager.getLights();
	m_numLights = MAX_LIGHTS < lightObjects.size() ? MAX_LIGHTS : lightObjects.size();

	glm::mat4 lightProjMat(glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, LIGHT_NEAR, LIGHT_FAR));

	for (int i = 0; i < m_numLights; ++i)
	{
		Light* light = lightObjects.at(i);
		if (light->isUpdated())
			continue;

		LightData& data = m_lightData[i];

		data.position = glm::vec4(light->m_position, light->m_isEnabled ? light->m_spotRadius : 1.0);
		data.color = glm::vec4(light->m_color, light->m_linearAttenuation);
		data.direction = glm::vec4(light->m_direction, light->m_spotDropoff);

		glm::mat4 lightViewMat = glm::lookAt(light->m_position, light->m_position + light->m_direction, glm::vec3(0, 1, 0));
		m_lightTransforms[i].VPMatrix = lightProjMat * lightViewMat;

		light->setUpdated(true);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_lightDataBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_numLights * sizeof(LightData), m_lightData);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_DATA_BINDING_POINT, m_lightDataBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, m_lightTransformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_numLights * sizeof(LightTransform), m_lightTransforms);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_TRANSFORM_BINDING_POINT, m_lightTransformBuffer);
}

void ForwardShader::setupShadowFramebuffer()
{
	glGenTextures(1, &m_shadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, MAX_LIGHTS,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// Create FBO to render depth into
	glGenFramebuffers(1, &m_shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	// Attach the depth texture to it
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowArrayTex, 0);
	// Disable color rendering as there are no color attachments
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ForwardShader::generateShadowMaps()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowArrayTex, 0);
	glUseProgram(m_shadowProgram);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ForwardShader::finishShadowMaps()
{
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}