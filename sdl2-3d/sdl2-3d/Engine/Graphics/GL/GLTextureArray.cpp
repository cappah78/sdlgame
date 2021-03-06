#include "GLTextureArray.h"

#include "../Pixmap.h"
#include <assert.h>
#include <string>

#include "../../Utils/CheckGLError.h"

GLTextureArray::GLTextureArray(const char** filePaths, unsigned int numTextures, unsigned int textureWidth, unsigned int textureHeight,
	bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, imageNames.size());
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureWidth, textureHeight, numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, textureWrapT);

	for (unsigned int i = 0; i < numTextures; ++i)
	{
		const char* imageName = filePaths[i];
		Pixmap p(imageName);
		assert(p.m_width == textureWidth && "Image width does not match with the width of this array");
		assert(p.m_height == textureHeight && "Image height does not match with the height of this array");
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, p.m_data);
	}

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

GLTextureArray::GLTextureArray(const std::vector<std::string>& imageNames, 
	unsigned int textureWidth, unsigned int textureHeight,
	bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	CHECK_GL_ERROR();
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, imageNames.size());
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureWidth, textureHeight, imageNames.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, textureWrapT);
	int i = 0;
	for (const std::string& imageName : imageNames)
	{
		Pixmap p(imageName.c_str());
		assert(p.m_width == textureWidth);
		assert(p.m_height == textureHeight);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, p.m_data);
		++i;
	}

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

GLTextureArray::~GLTextureArray()
{

}

void GLTextureArray::dispose()
{
	glDeleteTextures(1, &m_textureID);
}

void GLTextureArray::bind(IShader& shader, unsigned int index)
{
	bind(index);
}

void GLTextureArray::unbind(IShader& shader, unsigned int index)
{
	unbind(index);
}

inline void GLTextureArray::bind(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
}

inline void GLTextureArray::unbind(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}