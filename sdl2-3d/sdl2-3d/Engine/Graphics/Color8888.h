#ifndef COLOR8888_H_
#define COLOR8888_H_

/** Represents an rgba color with 8 bits per component*/
struct Color8888
{
	Color8888(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255) : r(r), g(g), b(b), a(a) {};

	/** Construct a color from 4 floats within the range of 0-1, no bounds checking is done*/
	/*
	Color8888(float r, float g, float b, float a)
		: r((unsigned char) (r * 255.0f))
		, g((unsigned char) (g * 255.0f))
		, b((unsigned char) (b * 255.0f))
		, a((unsigned char) (a * 255.0f)) 
	{};
	*/

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
