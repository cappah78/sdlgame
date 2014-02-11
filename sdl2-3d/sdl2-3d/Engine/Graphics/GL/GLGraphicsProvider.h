#pragma once

#include "../Model/IGraphicsProvider.h"

class GLGraphicsProvider : public IGraphicsProvider
{
public:
	GLGraphicsProvider();
	virtual ~GLGraphicsProvider() override;

	/** Simple version*/
	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath) override;
	/** Complex version */
	virtual std::auto_ptr<IShader> createShaderFromFile(const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, const char* geometryShaderFilePath, const char* pixelShaderFilePath) override;
	virtual std::auto_ptr<IShader> createComputeShaderFromFile(const char* computeShaderFilePath) override;

	virtual std::auto_ptr<ITexture> createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters) override;
	virtual std::auto_ptr<IVertexBuffer> createVertexBuffer(std::auto_ptr<IShader>& shader, const IVertexBufferParameters& parameters) override;
	virtual std::auto_ptr<IConstantBuffer> createConstantBuffer(std::auto_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters) override;
	virtual std::auto_ptr<IStateBuffer> createStateBuffer() override;
};