#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <gl\glew.h>

class GBuffer
{
public:
	enum GBufferTextureType {
		TEXCOORD, NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	bool init(unsigned int width, unsigned int height);
	void bindForWriting();
	void bindForReading();

	void setReadBuffer(GBufferTextureType textureType);

private:
	
	GLuint m_fbo;
	GLuint m_textures[NUM_TEXTURES];
	GLuint m_depthTexture;

};

#endif //GBUFFER_H_