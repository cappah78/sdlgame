#ifndef GBUFFER_H_
#define GBUFFER_H_

typedef unsigned int GLuint;

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

private:
	GLuint gBufferShader;

	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint depthBuffer;
	GLuint gBuffer;
};

#endif //GBUFFER_H_