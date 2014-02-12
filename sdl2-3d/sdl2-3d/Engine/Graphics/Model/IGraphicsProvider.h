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

class IIndiceBuffer;
struct IIndiceBufferParameters;

class IStateBuffer;

class IGraphicsProvider
{
public:
	IGraphicsProvider() {};
	virtual ~IGraphicsProvider() {};

	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath) = 0;
	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, const char* geometryShaderFilePath, const char* pixelShaderFilePath) = 0;
	virtual std::auto_ptr<IShader> createComputeShaderFromFile(const char* computeShaderFilePath) = 0;

	virtual std::auto_ptr<ITexture> createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters) = 0;

	virtual std::auto_ptr<IStateBuffer> createStateBuffer() = 0;

	virtual std::auto_ptr<IVertexBuffer> createVertexBuffer() = 0;
	virtual std::auto_ptr<IIndiceBuffer> createIndiceBuffer(const IIndiceBufferParameters& parameters) = 0;
	virtual std::auto_ptr<IConstantBuffer> createConstantBuffer(std::auto_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters) = 0;
};