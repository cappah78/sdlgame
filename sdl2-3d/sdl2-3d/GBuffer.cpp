#include "GBuffer.h"

const int windowWidth = 1024;
const int windowHeight = 1024;

#define ARRAY_SIZE_IN_ELEMENTS(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

GBuffer::GBuffer() {
	fbo = 0;
	depthTexture = 0;
	finalTexture = 0;
    memset(textures, 0, ARRAY_SIZE_IN_ELEMENTS(textures) * sizeof(GLuint));
}

GBuffer::~GBuffer() {
	if (fbo != 0)
		glDeleteFramebuffers(1, &fbo);
	if (textures[0] != 0)
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(textures), textures);
	if (depthTexture != 0)
		glDeleteTextures(1, &depthTexture);
}

bool GBuffer::init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, &fbo);    
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(textures), textures);

	glGenTextures(1, &depthTexture);

	glGenTextures(1, &finalTexture);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(textures) ; i++) {
    	glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
    }

	// depth
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// final
	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, 0);	

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}


void GBuffer::startFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForGeomPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, 
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2 };

    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawBuffers), drawBuffers);
}


void GBuffer::bindForStencilPass()
{
    // must disable the draw buffers 
	glDrawBuffer(GL_NONE);
}



void GBuffer::bindForLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(textures); i++) {
		glActiveTexture(GL_TEXTURE0 + i);		
		glBindTexture(GL_TEXTURE_2D, textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}


void GBuffer::bindForFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}