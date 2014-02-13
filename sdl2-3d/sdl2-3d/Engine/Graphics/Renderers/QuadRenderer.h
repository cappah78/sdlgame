#pragma once
#if 0	// depricated for now

#include <GL\glew.h>
#include "../GL/GLVertexBuffer.h"

static float QUAD_VERTICES[] = { 
	-1, 1, 0,
	-1, -1, 0,
	1, -1, 0,
	1, 1, 0
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

	GLVertexBuffer m_positionBuffer;
	GLVertexBuffer m_indiceBuffer;
};
#endif