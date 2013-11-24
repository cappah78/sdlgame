#ifndef PIXMAP_H_
#define PIXMAP_H_

/** Represents texture data on the cpu */
class Pixmap
{
public:
	Pixmap(const char* const fileName);
	~Pixmap();
	unsigned char* m_data;
	int m_width;
	int m_height;
	int m_numComponents;
};

#endif //PIXMAP_H_