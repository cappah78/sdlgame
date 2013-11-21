#ifndef TEXTURE_ARRAY_H_
#define TEXTURE_ARRAY_H_

typedef unsigned int GLuint;
typedef unsigned int GLenum;

#include <vector>

class TextureArray
{
public:
	TextureArray(const std::vector<const char*>& imageNames, unsigned int textureWidth, unsigned int textureHeight);
	TextureArray(const TextureArray& copyMe);
	~TextureArray();

	void bind() const;
	void bind(GLenum textureUnit) const;
	void dispose();

private:
	GLuint m_textureID;
};

#endif //TEXTURE_ARRAY_H_