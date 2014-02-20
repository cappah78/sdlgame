#pragma once

#include "ITexture.h"

class Pixmap;

struct ITextureArrayParameters
{
	const char** filePaths;
	unsigned int numTextures;
	unsigned int arrayWidth;
	unsigned int arrayHeight;
	TextureSettings textureSettings;
};

class ITextureArray
{
public:
	ITextureArray() {};
	virtual ~ITextureArray() {};

	virtual void bind() = 0;
private:
};