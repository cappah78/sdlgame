#include "TextureArray.h"

#include "../Pixmap.h"
#include <assert.h>
#include <string>

TextureArray::TextureArray(const std::vector<const char*>& imageNames, 
	unsigned int textureWidth, unsigned int textureHeight,
	bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, imageNames.size());
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureWidth, textureHeight, imageNames.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, textureWrapT);
	int i = 0;
	for (const char* imageName : imageNames)
	{
		Pixmap p(imageName);
		assert(p.m_width == textureWidth && "Image width does not match with the width of this array");
		assert(p.m_height == textureHeight && "Image height does not match with the height of this array");
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, p.m_data);
		++i;
	}

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

TextureArray::TextureArray(const std::vector<const std::string*>& imageNames, 
	unsigned int textureWidth, unsigned int textureHeight,
	bool generateMipMaps,
	GLint minFilter, GLint magFilter,
	GLint textureWrapS, GLint textureWrapT)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, imageNames.size());
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureWidth, textureHeight, imageNames.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, textureWrapT);
	int i = 0;
	for (const std::string* imageName : imageNames)
	{
		Pixmap p(imageName->c_str());
		assert(p.m_width == textureWidth);
		assert(p.m_height == textureHeight);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, p.m_data);
		++i;
	}

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

TextureArray::TextureArray(const TextureArray& copyMe)
{

}

TextureArray::~TextureArray()
{

}

void TextureArray::dispose()
{
	glDeleteTextures(1, &m_textureID);
}

void TextureArray::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
}

void TextureArray::bind(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
}
