#pragma once

class Pixmap;
class IShader;

struct TextureSettings
{
	enum FilterSettings { NEAREST, LINEAR, MIPMAP_NEAREST, MIPMAP_LINEAR };
	enum WrapSettings { CLAMP, REPEAT, MIRROR, WRAP };
	FilterSettings minFilter;
	FilterSettings magFilter;
	WrapSettings uWrap;
	WrapSettings vWrap;
};

struct ITextureParameters
{
	const char* filePath;
	TextureSettings textureSettings;
};

class ITexture
{
public:
	ITexture() {};
	virtual ~ITexture() {};
	virtual void bind(IShader& shader, unsigned int index = 0) = 0;
	virtual void unbind(IShader& shader, unsigned int index = 0) = 0;
};