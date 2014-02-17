#include "D3DGraphicsProvider.h"

#include "../Model/ITexture.h"
#include "../Model/IConstantBuffer.h"
#include "../Model/IVertexBuffer.h"
#include "../Model/IShader.h"
#include "../Model/IStateBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include "../D3D/D3DVertexBuffer.h"

#include "../../Utils/FileReader.h"


D3DGraphicsProvider::D3DGraphicsProvider()
{

}

D3DGraphicsProvider::~D3DGraphicsProvider()
{

}

std::unique_ptr<IShader> D3DGraphicsProvider::createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath)
{
	return std::auto_ptr<IShader>();
}

std::unique_ptr<IShader> D3DGraphicsProvider::createShaderFromFile(
	const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath,
	const char* geometryShaderFilePath, const char* pixelShaderFilePath)
{
	return std::auto_ptr<IShader>();
}

std::unique_ptr<IShader> D3DGraphicsProvider::createComputeShaderFromFile(const char* computeShaderFilePath)
{
	return std::auto_ptr<IShader>();
}

std::unique_ptr<ITexture> D3DGraphicsProvider::createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters)
{
	return std::auto_ptr<ITexture>();
}

std::unique_ptr<IVertexBuffer> D3DGraphicsProvider::createVertexBuffer(const IVertexBufferParameters& parameters)
{
	return std::auto_ptr<IVertexBuffer>(new D3DVertexBuffer(D3D11_BIND_VERTEX_BUFFER, parameters.m_sizeInBytes, parameters.m_data, parameters.m_drawMode));
}

std::unique_ptr<IIndiceBuffer> D3DGraphicsProvider::createIndiceBuffer(const IIndiceBufferParameters& parameters)
{
	return std::auto_ptr<IIndiceBuffer>(new D3DVertexBuffer(D3D11_BIND_INDEX_BUFFER, parameters.m_sizeInBytes, parameters.m_data, parameters.m_drawMode));
}

std::unique_ptr<IConstantBuffer> D3DGraphicsProvider::createConstantBuffer(std::unique_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters)
{
	return std::auto_ptr<IConstantBuffer>();
}

std::unique_ptr<IStateBuffer> D3DGraphicsProvider::createStateBuffer()
{
	return std::auto_ptr<IStateBuffer>();
}