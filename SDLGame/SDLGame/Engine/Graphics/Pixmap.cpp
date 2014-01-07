#include "Pixmap.h"

#include "..\Utils\stb_image.h"
#include <string>

Pixmap::Pixmap(const char* const fileName)
{
	m_data = stbi_load(fileName, &m_width, &m_height, &m_numComponents, 0);
}

Pixmap::Pixmap(const std::string* const fileName)
{
	m_data = stbi_load(fileName->c_str(), &m_width, &m_height, &m_numComponents, 0);
}

Pixmap::~Pixmap()
{
	delete m_data;
}