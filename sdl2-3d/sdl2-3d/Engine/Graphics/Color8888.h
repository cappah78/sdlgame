#ifndef COLOR8888_H_
#define COLOR8888_H_

#include <assert.h>

/** Represents an rgba color with 8 bits per component*/
struct Color8888
{
	Color8888(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) : r(r), g(g), b(b), a(a) {};

	unsigned r : 8;
	unsigned g : 8;
	unsigned b : 8;
	unsigned a : 8;

	unsigned int toIntBits()
	{
		unsigned int i = *((unsigned int*) this);
		return i;
	};

	/** Create a color from 4 floats within the range of 0-1 */
	static Color8888 fromFloats(float r, float g, float b, float a)
	{
		// Perhaps replace asserts with clamp to avoid floating point errors?
		assert(r >= 0.0f && r <= 1.0f);
		assert(g >= 0.0f && g <= 1.0f);
		assert(b >= 0.0f && b <= 1.0f);
		assert(a >= 0.0f && a <= 1.0f);

		Color8888 col(
			((unsigned char) (r * 255.0f)),
			((unsigned char) (g * 255.0f)),
			((unsigned char) (b * 255.0f)),
			((unsigned char) (a * 255.0f))
			);
		return col;
	}

	static Color8888 fromIntBits(unsigned int colorBits)
	{
		Color8888 col = *(Color8888*) colorBits;
		return col;
	}

	static unsigned int toIntBits(Color8888 col)
	{
		unsigned int i = *((unsigned int*) &col);
		return i;
	}
};


#endif //COLOR8888_H_
