#ifndef PIXMAP_H_
#define PIXMAP_H_

/** Represents texture data on the cpu */
class Pixmap
{
public:
	Pixmap(const char* fileName);
	~Pixmap();
	unsigned char* data;
	int width, height, numComponents;
};

#endif //PIXMAP_H_