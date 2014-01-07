#include "CubeMap.h"

#include <gl\glew.h>
#include <string>

#include "../Pixmap.h"

CubeMap::CubeMap(const char** textureNames)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	for (int i = 0; i<6; i++)
	{
		Pixmap img = Pixmap(textureNames[i]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.m_width, img.m_height, 0, img.m_numComponents == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img.m_data);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{

}

void CubeMap::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

void CubeMap::dispose()
{
	glDeleteTextures(1, &m_textureID);
}