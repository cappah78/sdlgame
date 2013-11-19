#ifndef TEXTUREREGION_H_
#define TEXTUREREGION_H_

#include <glm\glm.hpp>
#include "Texture.h"

static const glm::vec4 FULL_REGION(0.0f, 1.0f, 1.0f, 0.0f);

typedef unsigned int GLuint;

/** Represents a part of a texture */
struct TextureRegion
{
	TextureRegion(const Texture* texture, glm::vec4& texCoords = glm::vec4(FULL_REGION))
	: m_texture(*texture)
	, m_texCoords(texCoords)
	{};

	const Texture m_texture;
	const glm::vec4 m_texCoords;
};

#endif //TEXTUREREGION_H_