#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <gl\glew.h>
#include <vector>

class GBuffer
{
public:

	GBuffer(unsigned int numTextures);
	~GBuffer();

	bool init(unsigned int width, unsigned int height);
	void bindForWriting();
	void bindForReading();

	void setReadBuffer(unsigned int textureIdx);

private:
	unsigned int m_numTextures;

	GLuint m_fbo;
	std::vector<GLuint> m_textures;
	GLuint m_depthTexture;
};

#endif //GBUFFER_H_