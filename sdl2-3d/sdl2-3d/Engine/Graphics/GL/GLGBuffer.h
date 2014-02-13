#pragma once

#include <gl\glew.h>
#include <vector>

#include "../../../Game.h"


class GLGBuffer
{
public:

	GLGBuffer(unsigned int numTextures, unsigned int width = Game::graphics.getScreenWidth(), unsigned int height = Game::graphics.getScreenHeight());
	~GLGBuffer();

	void unbind();
	void bindForWriting();
	void bindForReading(unsigned int fromTextureUnit = 0);
	void bindDepthTexture(unsigned int textureUnit = 0);

	void setReadBuffer(unsigned int textureIdx);

	GLuint getDepthTexId() const { return m_depthTexture; };

private:
	bool init(unsigned int width, unsigned int height);

	unsigned int m_numTextures;

	GLuint m_fbo;
	std::vector<GLuint> m_textures;
	GLuint m_depthTexture;
};