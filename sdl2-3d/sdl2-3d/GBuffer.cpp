#include "GBuffer.h"

#include <stdio.h>
#include <iostream>

#include <gl/glew.h>

#include "ShaderManager.h"
#include "LightManager.h"
#include "Camera.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int SHADOW_MAP_SIZE = 1024;

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
{
	loadShaders();
	setupGBuffer();
	setupUniforms();

	// create uniform buffer and store camera data
	glGenBuffers(1, &transformUB);
	glBindBuffer(GL_UNIFORM_BUFFER, transformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(transform), &transform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, transformUB);


	glEnable(GL_FRAMEBUFFER_SRGB);
}

GBuffer::~GBuffer() 
{

}

void GBuffer::loadShaders()
{
	GBufferPPL = ShaderManager::createShaderProgram("gbuffer.vert", 0, "gbuffer.frag");
	//lightPPL = ShaderManager::createShaderProgram("light.vert", "light.geom", "light.frag");
	lightPassPPL = ShaderManager::createShaderProgram("lightpass.vert", "fullscreenquad.geom", "lightpass.frag");
	//shadowMultiPPL  = ShaderManager::createShaderProgram("shadowmulti.vert", "shadowmulti.geom", 0);
	//shadowSinglePPL = ShaderManager::createShaderProgram("shadowsingle.vert", 0, 0);
	//lightIDUniformLoc = glGetUniformLocation(shadowSinglePPL, "lightID");
}

void GBuffer::setupUniforms()
{
	glUseProgram(lightPassPPL);
	u_positionBufferLoc = glGetUniformLocation(lightPassPPL, "u_positionBuffer");
	glUniform1i(u_positionBufferLoc, 0);
	u_normalBufferLoc = glGetUniformLocation(lightPassPPL, "u_normalBuffer");
	glUniform1i(u_normalBufferLoc, 1);
	u_depthBufferLoc = glGetUniformLocation(lightPassPPL, "u_depthBuffer");
	glUniform1i(u_depthBufferLoc, 2);
	u_shadowMapArrayLoc = glGetUniformLocation(lightPassPPL, "u_shadowMapArray");
	glUniform1i(u_shadowMapArrayLoc, 3);

	std::cout << "Uniform locs: " << u_positionBufferLoc << ":" << u_normalBufferLoc << ":" << u_depthBufferLoc << std::endl;
}

void GBuffer::setupGBuffer()
{
	std::cerr << "> Setting up G-buffer..." << std::endl;
	// create normal buffer
	glGenTextures(1, &positionGBT);
	glBindTexture(GL_TEXTURE_2D, positionGBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// create framebuffer and setup attachments
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionGBT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalGBT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthGBT, 0);

	// create shadow map texture cube array
	glGenTextures(1, &shadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, LightManager::MAX_LIGHTS,
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
	transform.MVPMatrix = camera.combinedMatrix;
	transform.MVMatrix = camera.viewMatrix;
	transform.PMatrix = camera.projectionMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, transformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(transform), &transform, GL_STATIC_DRAW);

	// render G-buffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glUseProgram(GBufferPPL);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GBuffer::setupShadows()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowArrayTex, 0);
	glUseProgram(shadowMultiPPL);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GBuffer::drawBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(lightPassPPL);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, positionGBT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalGBT);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);

	glDrawArrays(GL_POINTS, 0, 1);
}

void GBuffer::renderLights()
{
// render lights
	glUseProgram(lightPPL);

	glBindBuffer(GL_UNIFORM_BUFFER, transformUB);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(transform), &transform, GL_STATIC_DRAW);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, positionGBT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalGBT);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthGBT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowArrayTex);

	glDrawArraysInstanced(GL_POINTS, 0, 1, LightManager::MAX_LIGHTS);
}