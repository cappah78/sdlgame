#pragma once

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