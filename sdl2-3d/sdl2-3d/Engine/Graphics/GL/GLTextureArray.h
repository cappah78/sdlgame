#pragma once

typedef unsigned int GLuint;
typedef unsigned int GLenum;

#include <vector>
#include <string>
#include <gl\glew.h>

/** Wrapper for OpenGL Texture Array */
class GLTextureArray
{
public:
	GLTextureArray(const std::vector<const char*>& imageNames, unsigned int textureWidth, unsigned int textureHeight, 
		bool generateMipMaps = true,
		GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_CLAMP_TO_EDGE, GLint textureWrapT = GL_CLAMP_TO_EDGE);

	GLTextureArray(const std::vector<std::string>& imageNames, unsigned int textureWidth, unsigned int textureHeight,
		bool generateMipMaps = true,
		GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_CLAMP_TO_EDGE, GLint textureWrapT = GL_CLAMP_TO_EDGE);

	GLTextureArray(const GLTextureArray& copyMe);
	~GLTextureArray();

	void bind() const;
	void bind(GLenum textureUnit) const;
	void dispose();

private:
	GLuint m_textureID;
};