#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <gl/glew.h>
#include <glm/glm.hpp>

class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	bool init(unsigned int width, unsigned int height);

	void bindForWriting();
	void bindForReading();

private:
	GLuint fbo;
	GLuint textures[GBUFFER_NUM_TEXTURES];
	GLuint depthTexture;
};

#endif //GBUFFER_H_