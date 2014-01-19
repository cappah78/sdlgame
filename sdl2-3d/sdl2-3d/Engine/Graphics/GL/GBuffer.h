#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <gl\glew.h>
#include <vector>

#include "../../../Game.h"


class GBuffer
{
public:

	GBuffer(unsigned int numTextures, unsigned int width = Game::graphics.getScreenWidth(), unsigned int height = Game::graphics.getScreenHeight());
	~GBuffer();

	void bindForWriting();
	void bindForReading();

	void setReadBuffer(unsigned int textureIdx);

private:
	bool init(unsigned int width, unsigned int height);

	unsigned int m_numTextures;

	GLuint m_fbo;
	std::vector<GLuint> m_textures;
	GLuint m_depthTexture;
};

#endif //GBUFFER_H_