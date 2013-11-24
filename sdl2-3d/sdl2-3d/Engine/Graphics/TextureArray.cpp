#include "TextureArray.h"

#include "Pixmap.h"

#include <gl\glew.h>
#include <assert.h>

TextureArray::TextureArray(const std::vector<const char*>& imageNames, unsigned int textureWidth, unsigned int textureHeight)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, imageNames.size());
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureWidth, textureHeight, imageNames.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	int i = 0;
	for (const char* imageName : imageNames)
	{
		Pixmap p(imageName);
		assert(p.m_width == textureWidth);
		assert(p.m_height == textureHeight);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, p.m_data);
		i++;
	}

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
