#ifndef TEXTURE_H_
#define TEXTURE_H_

class Pixmap;

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** Represents a texture on the gpu */
class Texture
{
public:
	Texture(const Pixmap& pixmap);
	Texture(const char* const fileName);
	Texture(const Pixmap& pixmap, GLuint& textureID);
	Texture(const char* const fileName, GLuint& textureID);
	~Texture();

	void bind() const;
	void bind(GLenum textureUnit) const;
	void dispose();

	const GLuint getTextureID() const
	{
		return m_textureID;
	}
	const unsigned int getWidth() const
	{
		return m_width;
	}
	const unsigned int getHeight() const
	{
		return m_height;
	}
	const unsigned char getNumComponents() const
	{
		return m_numComponents;
	}
private:

	void setupGLTexture(const Pixmap& pixmap);

	GLuint m_textureID;
	unsigned int m_width;
	unsigned int m_height;
	/** Amount of values per pixel (rgba = 4) */
	unsigned char m_numComponents;
};

#endif //TEXTURE_H_
