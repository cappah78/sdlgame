#include "GBuffer.h"

const int windowWidth = 1024;
const int windowHeight = 1024;

#define ARRAY_SIZE_IN_ELEMENTS(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

GBuffer::GBuffer() {
}

GBuffer::~GBuffer() {

}

bool GBuffer::init(unsigned int width, unsigned int height) 
{
// Create the FBO
    glGenFramebuffers(1, &fbo); 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(textures), textures);
    glGenTextures(1, &depthTexture);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(textures) ; i++) {
       glBindTexture(GL_TEXTURE_2D, textures[i]);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
       glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                  NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }; 
    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawBuffers), drawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}