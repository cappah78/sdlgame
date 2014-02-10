#pragma once

#include <memory>

class Pixmap;
class IShader;

class ITexture;
struct ITextureParameters;

class IConstantBuffer;
struct IConstantBufferParameters;

class IVertexBuffer;
struct IVertexBufferParameters;

class IStateBuffer;

class IGraphicsProvider
{
public:
	IGraphicsProvider() {};
	virtual ~IGraphicsProvider() {};

	/** Simple version*/
	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath) = 0;
	/** Complex version */
	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, const char* geometryShaderFilePath, const char* pixelShaderFilePath) = 0;
	virtual std::auto_ptr<IShader> createComputeShaderFromFile(const char* computeShaderFilePath) = 0;

	virtual std::auto_ptr<ITexture> createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters) = 0;
	virtual std::auto_ptr<IVertexBuffer> createVertexBuffer(IShader* shader, const IVertexBufferParameters& parameters) = 0;
	virtual std::auto_ptr<IConstantBuffer> createConstantBuffer(IShader* shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters) = 0;
	virtual std::auto_ptr<IStateBuffer> createStateBuffer() = 0;
};