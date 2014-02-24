#include "Pixmap.h"

//#define USE_STBI

#ifdef USE_STBI
#include "..\Utils\stb_image.h"
#else
#include <IL\il.h>
#endif

Pixmap::Pixmap(const char* const fileName)
{
#ifdef USE_STBI
	m_data = stbi_load(fileName, &m_width, &m_height, &m_numComponents, 0);
	if (!m_data)
		printf("Failed to load image: %s \n", fileName);
#else
	m_ilID = ilGenImage();
	ILboolean loaded = ilLoadImage((const wchar_t*) fileName);
	
	if (!loaded)
	{

		printf("Failed to load image: %s : %i \n", fileName, ilGetError());
		return;
	}

	m_width = ilGetInteger(IL_IMAGE_WIDTH);
	m_height = ilGetInteger(IL_IMAGE_HEIGHT);
	m_numComponents = ilGetInteger(IL_IMAGE_CHANNELS);

	/*	!!!!!! HACK !!!!!! opengl doesnt seem to like 2 component images */
	if (m_numComponents == 2)	
	{
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		m_numComponents = 3;
	}

	m_data = ilGetData();
	
#endif
	//printf("loadad:  %s %i %i %i \n", fileName, m_width, m_height, m_numComponents);
}

Pixmap::~Pixmap()
{
#ifdef USE_STBI
	delete m_data;
#else
	ilDeleteImage(m_ilID);
#endif
}