#ifndef TEXTUREREGION_H_
#define TEXTUREREGION_H_

#include <gl\GL.h>

/** Represents a part of a texture */
struct TextureRegion
{
	TextureRegion(Texture& texture, GLfloat u, GLfloat v, GLfloat u2, GLfloat v2)
		: texture(texture)
		, u(u)
		, v(v)
		, u2(u2)
		, v2(v2) {};
	Texture& texture;
	GLfloat u;
	GLfloat v;
	GLfloat u2;
	GLfloat v2;
};

#endif //TEXTUREREGION_H_