#ifndef QUAD_RENDERER_H_
#define QUAD_RENDERER_H_

#include <GL\glew.h>

static const float QUAD_VERTICES[] = { 
	1, -1, 0,	// bottom right rocner
	1, 1, 0,	//top right corner
	-1, 1, 0,	//top left corner
	-1, -1, 0	//bottom left corner
};

static const unsigned char QUAD_INDICES[] = { 
	0, 1, 2, // first triangle (bottom left - top left - top right)
	0, 2, 3 // second triangle (bottom left - top right - bottom right)
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
	GLuint m_positionBuffer;
	GLuint m_indiceBuffer;
};

#endif //QUAD_RENDERER_H_