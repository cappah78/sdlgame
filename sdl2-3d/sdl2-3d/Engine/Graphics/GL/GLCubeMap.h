#pragma once

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** 
Wrapper for OpenGL GLCubeMap textures
*/
class GLCubeMap
{
public:
	/** 
	Create a cube map using 6 cstrings texture file names, order:
	right, left, top, bottom, front, back
	*/
	GLCubeMap(const char** textureNames);
	~GLCubeMap();

	void bind(GLenum textureUnit);
	void dispose();
	GLuint getTextureID() { return m_textureID; };
private:
	GLuint m_textureID;
};