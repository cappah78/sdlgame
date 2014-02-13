#pragma once

class Pixmap;

struct TextureParameters
{
	enum FilterSettings { NEAREST, LINEAR, MIPMAP_NEAREST, MIPMAP_LINEAR };
	enum WrapSettings { CLAMP, REPEAT, MIRROR, WRAP };
	FilterSettings minFilter;
	FilterSettings magFilter;
	WrapSettings uWrap;
	WrapSettings vWrap;
};

class ITexture
{
public:
	ITexture() {};
	virtual ~ITexture() {};
};