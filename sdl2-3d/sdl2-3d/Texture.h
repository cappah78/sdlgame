#ifndef TEXTURE_H_
#define TEXTURE_H_

class Pixmap;

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** Represents a texture on the gpu */
class Texture
{
public:
	Texture(Pixmap& pixmap);
	Texture(const char* fileName);
	Texture(Pixmap& pixmap, GLuint& textureID);
	Texture(const char* fileName, GLuint& textureID);
	~Texture();

	void bind();
	void bind(GLenum textureUnit);

	const GLuint getTextureID()
	{
		return m_textureID;
	}
	const unsigned int& getWidth()
	{
		return m_width;
	}
	const unsigned int& getHeight()
	{
		return m_height;
	}
	const unsigned int& getNumComponents()
	{
		return m_numComponents;
	}
private:

	void setupGLTexture(Pixmap& pixmap);

	GLuint m_textureID;
	unsigned int m_width;
	unsigned int m_height;
	/** Amount of values per pixel (rgba = 4) */
	unsigned int m_numComponents;
};

#endif //TEXTURE_H_
