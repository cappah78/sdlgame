#pragma once

class Pixmap;

#include <gl\glew.h>

#include "../Model/ITexture.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** Represents a GLTexture on the gpu */
class GLTexture : public ITexture
{
public:
	GLTexture() : m_textureID(0), m_width(0), m_height(0), m_numComponents(0) {};
	GLTexture(const Pixmap& pixmap, bool generateMipMaps = false,
		GLint minFilter = GL_LINEAR, GLint magFilter = GL_NEAREST,
		GLint GLTextureWrapS = GL_REPEAT, GLint GLTextureWrapT = GL_REPEAT);

	GLTexture(const char* const fileName, bool generateMipMaps = false,
		GLint minFilter = GL_LINEAR, GLint magFilter = GL_NEAREST,
		GLint GLTextureWrapS = GL_REPEAT, GLint GLTextureWrapT = GL_REPEAT);

	GLTexture(const GLTexture& copy) = delete;

	virtual ~GLTexture() override;

	bool isLoaded() const { return m_textureID != 0; };
	virtual void bind(unsigned int index) const override;
	virtual void unbind(unsigned int index) const override;

	GLuint getTextureID() const { return m_textureID; }
	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	unsigned char getNumComponents() const { return m_numComponents; }
private:
	void setupGLTexture(const Pixmap& pixmap, bool generateMipMaps, GLint minFilter, GLint magFilter,
		GLint GLTextureWrapS, GLint GLTextureWrapT);

	/** OpenGL GLTextureID*/
	GLuint m_textureID;
	/** width in pixels*/
	unsigned int m_width;
	/** height im pixels*/
	unsigned int m_height;
	/** Amount of values per pixel (rgba = 4) */
	unsigned char m_numComponents;
};