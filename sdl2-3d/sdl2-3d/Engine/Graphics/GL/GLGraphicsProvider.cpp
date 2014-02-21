#include "GLGraphicsProvider.h"

#include "../Model/ITexture.h"
#include "../Model/IConstantBuffer.h"
#include "../Model/IVertexBuffer.h"
#include "../Model/IShader.h"
#include "../Model/IStateBuffer.h"
#include "../Model/IIndiceBuffer.h"

#include "GLVertexBuffer.h"
#include "GLConstantBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLTextureArray.h"
#include "GLStateBuffer.h"

#include "../Model/EDrawMode.h"

#include "../../Utils/FileReader.h"
#include "../../Utils/CheckGLError.h"

GLGraphicsProvider::GLGraphicsProvider()
{

}

GLGraphicsProvider::~GLGraphicsProvider()
{
	for (auto it = m_loadedTextureMap.begin(); it != m_loadedTextureMap.end(); ++it)
	{
		delete it->second;
	}
}

void attachShaderSource(GLuint prog, GLenum type, const char * source)
{
	GLuint sh;

	sh = glCreateShader(type);

	if (sh == 0)
	{
		printf("Could not create shader: %s \n", type);
	}

	glShaderSource(sh, 1, &source, NULL);
	glCompileShader(sh);

	GLint logLen;
	glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 1)
	{
		char buffer[4096];
		glGetShaderInfoLog(sh, sizeof(buffer), NULL, buffer);
		char* typeString;
		switch (type) {
		case GL_FRAGMENT_SHADER:
			typeString = "fragment";
			break;
		case GL_VERTEX_SHADER:
			typeString = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			typeString = "geometry";
			break;
		case GL_TESS_CONTROL_SHADER:
			typeString = "tesselation control";
			break;
		case GL_TESS_EVALUATION_SHADER:
			typeString = "tesselation evaluation";
			break;
		default:
			typeString = "unknown";
			break;
		}
		printf("Error in %s shader: %s \n", typeString, buffer);
	}

	glAttachShader(prog, sh);
	glDeleteShader(sh);
	CHECK_GL_ERROR();
}

GLenum eDrawModeToGLDrawMode(EDrawMode drawMode)
{
	switch (drawMode)
	{
	case EDrawMode::STATIC:
		return GL_STATIC_DRAW;
	case EDrawMode::DYNAMIC:
		return GL_DYNAMIC_DRAW;
	case EDrawMode::STREAM:
		return GL_STREAM_DRAW;
	default:
		assert(false);
		return GL_STATIC_DRAW;
	}
}

std::unique_ptr<IShader> GLGraphicsProvider::createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath)
{
	assert(vertexShaderFilePath && pixelShaderFilePath);
	CHECK_GL_ERROR();

	GLuint program = glCreateProgram();
	std::string vertexShaderContents = FileReader::readStringFromFile(vertexShaderFilePath);
	attachShaderSource(program, GL_VERTEX_SHADER, vertexShaderContents.c_str());

	std::string fragmentShaderContents = FileReader::readStringFromFile(pixelShaderFilePath);
	attachShaderSource(program, GL_FRAGMENT_SHADER, fragmentShaderContents.c_str());

	glLinkProgram(program);

	IShader* shader = new GLShader(program);
	CHECK_GL_ERROR();
	return std::unique_ptr<IShader>(shader);
}

std::unique_ptr<IShader> GLGraphicsProvider::createShaderFromFile(
	const char* vertexShaderFilePath, const char* hullShaderFilePath, const char* domainShaderFilePath, 
	const char* geometryShaderFilePath, const char* pixelShaderFilePath)
{
	GLuint program = glCreateProgram();

	if (vertexShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(vertexShaderFilePath);
		attachShaderSource(program, GL_VERTEX_SHADER, contents.c_str());
	}
	if (hullShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(hullShaderFilePath);
		attachShaderSource(program, GL_TESS_CONTROL_SHADER, contents.c_str());
	}
	if (domainShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(domainShaderFilePath);
		attachShaderSource(program, GL_TESS_EVALUATION_SHADER, contents.c_str());
	}
	if (geometryShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(domainShaderFilePath);
		attachShaderSource(program, GL_GEOMETRY_SHADER, contents.c_str());
	}
	if (pixelShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(pixelShaderFilePath);
		attachShaderSource(program, GL_FRAGMENT_SHADER, contents.c_str());
	}
	glLinkProgram(program);

	IShader* shader = new GLShader(program);
	CHECK_GL_ERROR();
	return std::unique_ptr<IShader>(shader);
}

std::unique_ptr<IShader> GLGraphicsProvider::createComputeShaderFromFile(const char* computeShaderFilePath)
{
	GLuint program = glCreateProgram();
	if (computeShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(computeShaderFilePath);
		attachShaderSource(program, GL_COMPUTE_SHADER, contents.c_str());
	}
	glLinkProgram(program);

	IShader* shader = new GLShader(program);
	CHECK_GL_ERROR();
	return std::unique_ptr<IShader>(shader);
}

std::unique_ptr<ITexture> GLGraphicsProvider::createTexture(const ITextureParameters& parameters)
{
	//TODO: filter settings
	GLTexture* texture = new GLTexture(parameters.filePath);
	assert(texture->isLoaded());
	std::string textureFilePathString(parameters.filePath);
	m_loadedTextureMap.insert(std::make_pair(textureFilePathString, texture));

	return std::unique_ptr<ITexture>(texture);
}

std::unique_ptr<ITextureArray> GLGraphicsProvider::createTextureArray(const ITextureArrayParameters& parameters)
{
	//TODO: filter settings
	ITextureArray* textureArray = new GLTextureArray(parameters.filePaths, parameters.numTextures, parameters.arrayWidth, parameters.arrayHeight);
	CHECK_GL_ERROR();
	return std::unique_ptr<ITextureArray>(textureArray);
}

const ITexture* GLGraphicsProvider::getManagedTexture(const char* filePath)
{
	std::string filePathString(filePath);
	auto it = m_loadedTextureMap.find(filePathString);
	if (it == m_loadedTextureMap.end())
	{
		GLTexture* texture = new GLTexture(filePath);
		assert(texture->isLoaded());
		m_loadedTextureMap.insert(std::make_pair(filePathString, texture));
		return texture;
	}
	else
	{
		return it->second;
	}
}

std::unique_ptr<IVertexBuffer> GLGraphicsProvider::createVertexBuffer(const IVertexBufferParameters& parameters)
{	
	IVertexBuffer* vertexBuffer = new GLVertexBuffer(parameters.m_sizeInBytes, parameters.m_data, GL_ARRAY_BUFFER, eDrawModeToGLDrawMode(parameters.m_drawMode));
	CHECK_GL_ERROR();
	return std::unique_ptr<IVertexBuffer>(vertexBuffer);
}

std::unique_ptr<IIndiceBuffer> GLGraphicsProvider::createIndiceBuffer(const IIndiceBufferParameters& parameters)
{
	IIndiceBuffer* indiceBuffer = new GLVertexBuffer(parameters.m_sizeInBytes, parameters.m_data, GL_ELEMENT_ARRAY_BUFFER, eDrawModeToGLDrawMode(parameters.m_drawMode));
	CHECK_GL_ERROR();
	return std::unique_ptr<IIndiceBuffer>(indiceBuffer);
}

std::unique_ptr<IConstantBuffer> GLGraphicsProvider::createConstantBuffer(std::unique_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters)
{
	IConstantBuffer* constantBuffer = new GLConstantBuffer(shader->getID(), bufferIndex, bufferName);
	CHECK_GL_ERROR();
	return std::unique_ptr<IConstantBuffer>(constantBuffer);
}

std::unique_ptr<IStateBuffer> GLGraphicsProvider::createStateBuffer()
{
	IStateBuffer* stateBuffer = new GLStateBuffer();
	CHECK_GL_ERROR();
	return std::unique_ptr<IStateBuffer>(stateBuffer);
}