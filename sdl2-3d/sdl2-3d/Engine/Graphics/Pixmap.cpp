#include "Pixmap.h"

//#include "..\Utils\stb_image.h"
#include <IL\il.h>

Pixmap::Pixmap(const char* const fileName)
{
	/*
	m_data = stbi_load(fileName, &m_width, &m_height, &m_numComponents, 0);
	if (!m_data)
		printf("Failed to load image: %s \n", fileName);
	*/
	m_ilID = ilGenImage();
	ILboolean loaded = ilLoadImage((const wchar_t*) fileName);
	if (!loaded)
	{
		printf("Failed to load image: %s \n", fileName);
		return;
	}

	m_width = ilGetInteger(IL_IMAGE_WIDTH);
	m_height = ilGetInteger(IL_IMAGE_HEIGHT);
	m_numComponents = ilGetInteger(IL_IMAGE_CHANNELS);
	m_data = ilGetData();
}

Pixmap::~Pixmap()
{
	ilDeleteImage(m_ilID);
	//delete m_data;
}