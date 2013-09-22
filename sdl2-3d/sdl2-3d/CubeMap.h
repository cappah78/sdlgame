#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class CubeMap
{
public:
	static CubeMap* createFromTextures(std::string* textures);
	static CubeMap* createShadowMap(unsigned int resolution);

	~CubeMap();

	void bind(GLenum textureUnit);
private:
	CubeMap(GLuint texture);

	GLuint m_texture;
};

#endif //CUBE_MAP_H