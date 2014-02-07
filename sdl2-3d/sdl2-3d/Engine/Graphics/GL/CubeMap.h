#pragma once

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** 
Wrapper for OpenGL CubeMap textures
*/
class CubeMap
{
public:
	/** 
	Create a cube map using 6 cstrings texture file names, order:
	right, left, top, bottom, front, back
	*/
	CubeMap(const char** textureNames);
	~CubeMap();

	void bind(GLenum textureUnit);
	void dispose();
	GLuint getTextureID() { return m_textureID; };
private:
	GLuint m_textureID;
};