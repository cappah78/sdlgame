#ifndef SHADOW_CUBE_MAP_H_
#define SHADOW_CUBE_MAP_H_

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class ShadowCubeMap
{
public:
	ShadowCubeMap(unsigned int resolution);
	~ShadowCubeMap();

	void bind(GLenum textureUnit);
	void dispose();
	unsigned int getTextureID() { return m_textureID; };
private:
	GLuint m_textureID;
};

#endif //SHADOW_CUBE_MAP_H_