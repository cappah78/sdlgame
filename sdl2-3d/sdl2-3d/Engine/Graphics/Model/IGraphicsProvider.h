#pragma once

#include <memory>

class Pixmap;
class IShader;

class ITexture;
struct ITextureParameters;

class ITextureArray;
struct ITextureArrayParameters;

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

	virtual std::unique_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath) = 0;
	virtual std::unique_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, const char* geometryShaderFilePath, const char* pixelShaderFilePath) = 0;
	virtual std::unique_ptr<IShader> createComputeShaderFromFile(const char* computeShaderFilePath) = 0;

	virtual std::unique_ptr<ITexture> createTexture(const ITextureParameters& parameters) = 0;
	virtual std::unique_ptr<ITextureArray> createTextureArray(const ITextureArrayParameters& parameters) = 0;

	virtual const ITexture* getManagedTexture(const char* filePath) = 0;

	virtual std::unique_ptr<IStateBuffer> createStateBuffer() = 0;

	virtual std::unique_ptr<IVertexBuffer> createVertexBuffer(const IVertexBufferParameters& parameters) = 0;
	virtual std::unique_ptr<IIndiceBuffer> createIndiceBuffer(const IIndiceBufferParameters& parameters) = 0;
	virtual std::unique_ptr<IConstantBuffer> createConstantBuffer(std::unique_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters) = 0;
};