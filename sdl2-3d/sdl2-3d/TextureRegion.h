#ifndef TEXTUREREGION_H_
#define TEXTUREREGION_H_

typedef float GLfloat;

/** Represents a part of a texture */
struct TextureRegion
{
	TextureRegion(Texture& texture, GLfloat u, GLfloat v, GLfloat u2, GLfloat v2)
		: m_texture(texture)
		, u(u)
		, v(v)
		, u2(u2)
		, v2(v2) {};
	Texture& m_texture;
	GLfloat u;
	GLfloat v;
	GLfloat u2;
	GLfloat v2;
};

#endif //TEXTUREREGION_H_