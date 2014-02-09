#pragma once

class Pixmap;

#include <gl\glew.h>

#include "../Model/ITexture.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** Represents a texture on the gpu */
class Texture : public ITexture
{
public:
	Texture() : m_textureID(0), m_width(0), m_height(0), m_numComponents(0) {};
	Texture(const Pixmap& pixmap, bool generateMipMaps = false,
		GLint minFilter = GL_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_REPEAT, GLint textureWrapT = GL_REPEAT);

	Texture(const char* const fileName, bool generateMipMaps = false,
		GLint minFilter = GL_LINEAR, GLint magFilter = GL_NEAREST,
		GLint textureWrapS = GL_REPEAT, GLint textureWrapT = GL_REPEAT);

	virtual ~Texture() override;

	bool isLoaded() const { return m_textureID != 0; };
	void bind() const;
	void bind(GLenum textureUnit) const;

	GLuint getTextureID() const { return m_textureID; }
	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	unsigned char getNumComponents() const { return m_numComponents; }
private:
	void setupGLTexture(const Pixmap& pixmap, bool generateMipMaps, GLint minFilter, GLint magFilter,
		GLint textureWrapS, GLint textureWrapT);

	/** OpenGL TextureID*/
	GLuint m_textureID;
	/** width in pixels*/
	unsigned int m_width;
	/** height im pixels*/
	unsigned int m_height;
	/** Amount of values per pixel (rgba = 4) */
	unsigned char m_numComponents;
};