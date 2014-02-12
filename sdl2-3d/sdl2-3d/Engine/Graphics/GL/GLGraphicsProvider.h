#pragma once

#include "../Model/IGraphicsProvider.h"

class GLGraphicsProvider : public IGraphicsProvider
{
public:
	GLGraphicsProvider();
	virtual ~GLGraphicsProvider() override;

	virtual std::unique_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath) override;
	virtual std::unique_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, const char* geometryShaderFilePath, const char* pixelShaderFilePath) override;
	virtual std::unique_ptr<IShader> createComputeShaderFromFile(const char* computeShaderFilePath) override;

	virtual std::unique_ptr<ITexture> createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters) override;
	virtual std::unique_ptr<IVertexBuffer> createVertexBuffer() override;
	virtual std::unique_ptr<IConstantBuffer> createConstantBuffer(std::unique_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters) override;
	virtual std::unique_ptr<IIndiceBuffer> createIndiceBuffer(const IIndiceBufferParameters& parameters) override;
	virtual std::unique_ptr<IStateBuffer> createStateBuffer() override;
};