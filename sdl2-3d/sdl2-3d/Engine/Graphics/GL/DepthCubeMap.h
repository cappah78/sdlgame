#ifndef DEPTH_CUBE_MAP_H
#define DEPTH_CUBE_MAP_H

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class DepthCubeMap
{
public:
	DepthCubeMap();
	~DepthCubeMap();

	void bind(GLenum textureUnit);
	void dispose();
	GLuint getTextureID() { return m_textureID; };
private:
	GLuint m_textureID;
};

#endif //DEPTH_CUBE_MAP_H