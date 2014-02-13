#include "Pixmap.h"

#include "..\Utils\stb_image.h"

Pixmap::Pixmap(const char* const fileName)
{
	m_data = stbi_load(fileName, &m_width, &m_height, &m_numComponents, 0);
	if (!m_data)
		printf("Failed to load image: %s", fileName);
}

Pixmap::~Pixmap()
{
	delete m_data;
}