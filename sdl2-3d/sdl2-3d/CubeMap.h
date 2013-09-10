#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include <gl\glew.h>
#include <string>

class CubeMap
{
public:
	static CubeMap* createFromTextures(std::string* textures);
	static CubeMap* createShadowMap(unsigned int resolution);

	~CubeMap();
	void bind(GLenum textureUnit);
private:
	CubeMap(GLuint texture);
	GLuint texture;
};

#endif //CUBEMAP_H_