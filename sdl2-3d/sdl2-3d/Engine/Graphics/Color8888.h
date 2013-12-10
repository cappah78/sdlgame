#ifndef COLOR8888_H_
#define COLOR8888_H_

struct Color8888
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	unsigned int toIntBits()
	{
		unsigned int i = *((unsigned int*) this);
		return i;
	};
};


#endif //COLOR8888_H_
