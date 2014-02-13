#pragma once

/** Represents texture data on the cpu */
class Pixmap
{
public:
	Pixmap(const char* const fileName);
	~Pixmap();
	/** 
	Byte data of the image
	*/
	unsigned char* m_data;
	/** Width of the image in pixels*/
	int m_width;
	/** Height of the image in pixels*/
	int m_height;
	/** Number of components per pixel (e.g. rgba == 4)*/
	int m_numComponents;
};