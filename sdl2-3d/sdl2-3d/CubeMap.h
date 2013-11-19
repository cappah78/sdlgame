#ifndef CUBE_MAP_H
#define CUBE_MAP_H

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class CubeMap
{
public:
	CubeMap(const char** textureNames);
	~CubeMap();

	void bind(GLenum textureUnit);
	void dispose();
	GLuint getTextureID() { return m_textureID; };
private:

	GLuint m_textureID;
};

#endif //CUBE_MAP_H