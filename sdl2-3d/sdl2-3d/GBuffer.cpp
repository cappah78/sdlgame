#include "GBuffer.h"

#include <stdio.h>
#include <iostream>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "ShaderManager.h"


const int windowWidth = 1024;
const int windowHeight = 1024;

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

#define GBUFFER_VERTEX_SHADER "gbuffer.vert"
#define GBUFFER_FRAGMENT_SHADER "gbuffer.frag"

GBuffer::GBuffer()
{

	// create normal buffer
	glGenTextures(1, &positionBuffer);
	glBindTexture(GL_TEXTURE_2D, positionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create normal buffer
	glGenTextures(1, &normalBuffer);
	glBindTexture(GL_TEXTURE_2D, normalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create depth buffer
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// create framebuffer and setup attachments
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	const GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	CHECK_FRAMEBUFFER_STATUS();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CHECK_GL_ERROR();
}

GBuffer::~GBuffer() 
{

}