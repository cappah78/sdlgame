#ifndef QUAD_RENDERER_H_
#define QUAD_RENDERER_H_

#include <GL\glew.h>
#include "../GL/VertexBuffer.h"

static float QUAD_VERTICES[] = { 
	-1, 1, 0,
	-1, -1, 0,
	1, -1, 0,
	1, 1, 0
};

static float QUAD_TEXCOORDS [] = {
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0
};

static unsigned char QUAD_INDICES[] = { 
	0, 1, 2,
	0, 2, 3
}; 

class QuadRenderer
{
public:
	QuadRenderer();
	~QuadRenderer();
	QuadRenderer(const QuadRenderer& copy) = delete;

	void drawQuad();
private:
	GLuint m_vao;

	VertexBuffer<float> m_positionBuffer;
	VertexBuffer<float> m_texcoordBuffer;
	VertexBuffer<unsigned char> m_indiceBuffer;
};

#endif //QUAD_RENDERER_H_