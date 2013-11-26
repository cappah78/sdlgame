#include "ForwardShader.h"

#include "..\..\Utils\ShaderManager.h"
#include "..\Lighting\PointLight.h"
#include "../../Utils/CheckGLError.h"

#include <iostream>
#include <stdio.h>

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const int SHADOW_MAP_SIZE = 1024;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int CAMERA_TRANSFORM_BINDING_POINT = 0;
const int POINT_LIGHT_DATA_BINDING_POINT = 1;
const int SPOT_LIGHT_DATA_BINDING_POINT = 2;
const int DIRECTIONAL_LIGHT_DATA_BINDING_POINT = 3;

const float LIGHT_NEAR = 1.0f;
const float LIGHT_FAR = 1200.0f;

ForwardShader::ForwardShader()
{
	CHECK_GL_ERROR();
	m_forwardShaderProgram = ShaderManager::createShaderProgram("Shaders/forwardshader.vert", 0, "Shaders/forwardshader.frag");
	m_shadowProgram = ShaderManager::createShaderProgram("Shaders/shadowmulti.vert", "Shaders/shadowmulti.geom", 0);
	setupUniforms();
	setupBuffers();
	//setupShadowFramebuffer();
}

ForwardShader::~ForwardShader()
{

}

void ForwardShader::setupUniforms()
{
	glUseProgram(m_forwardShaderProgram);
	CHECK_GL_ERROR();

	GLint m_texLoc = glGetUniformLocation(m_forwardShaderProgram, "tex");
	glUniform1i(m_texLoc, 0);
	CHECK_GL_ERROR();

	m_ambientLightUniformLoc = glGetUniformLocation(m_forwardShaderProgram, "u_ambient");
	CHECK_GL_ERROR();

	GLuint cameraTransformIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "CameraTransform");
	glUniformBlockBinding(m_forwardShaderProgram, cameraTransformIdx, CAMERA_TRANSFORM_BINDING_POINT);
	CHECK_GL_ERROR();

	GLuint pointLightIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "PointLights");
	glUniformBlockBinding(m_forwardShaderProgram, pointLightIdx, POINT_LIGHT_DATA_BINDING_POINT);
	CHECK_GL_ERROR();

	GLuint spotLightIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "SpotLights");
	glUniformBlockBinding(m_forwardShaderProgram, spotLightIdx, SPOT_LIGHT_DATA_BINDING_POINT);
	CHECK_GL_ERROR();

	GLuint directionalLightIdx = glGetUniformBlockIndex(m_forwardShaderProgram, "DirectionalLights");
	glUniformBlockBinding(m_forwardShaderProgram, directionalLightIdx, DIRECTIONAL_LIGHT_DATA_BINDING_POINT);
	CHECK_GL_ERROR();
}

void ForwardShader::setupBuffers()
{
	glGenBuffers(1, &m_cameraTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);
	CHECK_GL_ERROR();

	glGenBuffers(1, &m_pointLightDataBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_pointLightDataBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, POINT_LIGHT_DATA_BINDING_POINT, m_pointLightDataBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_pointLightData), &m_pointLightData[0], GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &m_spotLightDataBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_spotLightDataBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, SPOT_LIGHT_DATA_BINDING_POINT, m_spotLightDataBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_spotLightData), &m_spotLightData[0], GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &m_directionalLightDataBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_directionalLightDataBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, DIRECTIONAL_LIGHT_DATA_BINDING_POINT, m_directionalLightDataBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_directionalLightData), &m_directionalLightData[0], GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

/*
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
*/

void ForwardShader::use(const Camera& camera)
{
	m_cameraTransform.VPMatrix = camera.m_combinedMatrix;
	m_cameraTransform.VMatrix = camera.m_viewMatrix;
	m_cameraTransform.PMatrix = camera.m_projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);

	glUseProgram(m_forwardShaderProgram);
}

void ForwardShader::updateLights(const Camera& camera, LightManager& lightManager)
{
	PointLights& pointLights = lightManager.getPointLights();
	SpotLights& spotLights = lightManager.getSpotLights();
	DirectionalLights& directionalLights = lightManager.getDirectionalLights();
	AmbientLights& ambientLights = lightManager.getAmbientLights();

	//smallest of MAX_POINT_LIGHTS and pointLights.size()
	int numPointLights = pointLights.size() < MAX_POINT_LIGHTS ? pointLights.size() : MAX_POINT_LIGHTS;

	for (int i = 0; i < numPointLights; ++i)
	{
		PointLight* light = pointLights.at(i);
		if (light->m_updated)
			continue;
		light->m_updated = true;

		m_pointLightData[i] = light->getData();
		glBindBuffer(GL_UNIFORM_BUFFER, m_pointLightDataBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightData), sizeof(PointLightData), &m_pointLightData[i]);
	}

	
	int numSpotLights = spotLights.size() < MAX_SPOT_LIGHTS ? spotLights.size() : MAX_SPOT_LIGHTS;
	for (int i = 0; i < numSpotLights; ++i)
	{
		SpotLight* light = spotLights.at(i);
		if (light->m_updated)
			continue;
		light->m_updated = true;

		m_spotLightData[i] = light->getData();
		glBindBuffer(GL_UNIFORM_BUFFER, m_spotLightDataBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(SpotLightData), sizeof(SpotLightData), &m_spotLightData[i]);
	}
	
	int numDirectionalLights = directionalLights.size() < MAX_DIRECTIONAL_LIGHTS ? directionalLights.size() : MAX_DIRECTIONAL_LIGHTS;
	for (int i = 0; i < numDirectionalLights; ++i)
	{
		DirectionalLight* light = directionalLights.at(i);
		if (light->m_updated)
			continue;
		light->m_updated = true;

		m_directionalLightData[i] = light->getData();
		glBindBuffer(GL_UNIFORM_BUFFER, m_directionalLightDataBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(DirectionalLightData), sizeof(DirectionalLightData), &m_directionalLightData[i]);
	}

	glm::vec3 ambient = glm::vec3(0);
	for (unsigned int i = 0; i < ambientLights.size(); ++i)
	{
		AmbientLight* light = ambientLights.at(i);
		if (!light->m_enabled)
			continue;

		ambient += light->m_color;
	}
	glUniform3f(m_ambientLightUniformLoc, ambient.r, ambient.g, ambient.b);
}

/*
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
*/