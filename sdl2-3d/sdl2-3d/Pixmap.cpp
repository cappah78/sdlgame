#include "Pixmap.h"

#include "stb_image.h"

Pixmap::Pixmap(const char* fileName)
{
	data = stbi_load(fileName, &width, &height, &numComponents, 0);
}

Pixmap::~Pixmap()
{
	delete data;
}