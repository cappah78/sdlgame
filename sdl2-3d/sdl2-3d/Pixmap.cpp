#include "Pixmap.h"

#include "stb_image.h"

Pixmap::Pixmap(const char* fileName)
{
	m_data = stbi_load(fileName, &m_width, &m_height, &m_numComponents, 0);
}

Pixmap::~Pixmap()
{
	delete m_data;
}