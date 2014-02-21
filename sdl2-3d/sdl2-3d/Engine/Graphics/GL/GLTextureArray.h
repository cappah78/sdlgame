#pragma once

#include <vector>
#include <string>
#include <gl\glew.h>

#include "../Model/ITextureArray.h"

/** Wrapper for OpenGL Texture Array */
class GLTextureArray : public ITextureArray
{
public:
	GLTextureArray(const char** filePaths, unsigned int numTextures, unsigned int textureWidth, unsigned int textureHeight, 
		bool generateMipMaps = true,
		GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_CLAMP_TO_EDGE, GLint textureWrapT = GL_CLAMP_TO_EDGE);

	GLTextureArray(const std::vector<std::string>& imageNames, unsigned int textureWidth, unsigned int textureHeight,
		bool generateMipMaps = true,
		GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_CLAMP_TO_EDGE, GLint textureWrapT = GL_CLAMP_TO_EDGE);

	GLTextureArray(const GLTextureArray& copyMe) = delete;
	~GLTextureArray();

	virtual void bind() override;

	void bind(GLenum textureUnit) const;
	void dispose();

private:
	GLuint m_textureID;
};