#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Pixmap.h"
#include <gl\glew.h>

/** Represents a texture on the gpu */
class Texture
{
public:
	Texture(Pixmap& pixmap);
	Texture(const char* fileName);
	Texture(Pixmap& pixmap, GLuint& textureID_);
	Texture(const char* fileName, GLuint& textureID_);
	~Texture();

	void bind();
	void bind(GLenum textureUnit);

	const GLuint getTextureID()
	{
		return textureID;
	}
	const unsigned int& getWidth()
	{
		return width;
	}
	const unsigned int& getHeight()
	{
		return height;
	}
	const unsigned int& getNumComponents()
	{
		return numComponents;
	}
private:

	void setupGLTexture(Pixmap& pixmap);

	GLuint textureID;
	unsigned int width;
	unsigned int height;
	/** Amount of values per pixel (rgba = 4) */
	unsigned int numComponents;
};

#endif TEXTURE_H_