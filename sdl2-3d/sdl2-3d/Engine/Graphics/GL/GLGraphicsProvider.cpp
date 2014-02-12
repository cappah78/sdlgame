#include "GLGraphicsProvider.h"

#include "../Model/ITexture.h"
#include "../Model/IConstantBuffer.h"
#include "../Model/IVertexBuffer.h"
#include "../Model/IShader.h"

#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "StateBuffer.h"

#include "../../Utils/FileReader.h"

GLGraphicsProvider::GLGraphicsProvider()
{

}

GLGraphicsProvider::~GLGraphicsProvider()
{

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
}

std::auto_ptr<IShader> GLGraphicsProvider::createShaderFromFile(const char* vertexShaderFilePath, const char* pixelShaderFilePath)
{
	assert(vertexShaderFilePath && pixelShaderFilePath);
	CHECK_GL_ERROR();

	GLuint program = glCreateProgram();
	std::string vertexShaderContents = FileReader::readStringFromFile(vertexShaderFilePath);
	attachShaderSource(program, GL_VERTEX_SHADER, vertexShaderContents.c_str());

	std::string fragmentShaderContents = FileReader::readStringFromFile(pixelShaderFilePath);
	attachShaderSource(program, GL_FRAGMENT_SHADER, fragmentShaderContents.c_str());

	glLinkProgram(program);

	return std::auto_ptr<IShader>(new Shader(program));
}
std::auto_ptr<IShader> GLGraphicsProvider::createShaderFromFile(
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

	return std::auto_ptr<IShader>(new Shader(program));
}

std::auto_ptr<IShader> GLGraphicsProvider::createComputeShaderFromFile(const char* computeShaderFilePath)
{
	GLuint program = glCreateProgram();
	if (computeShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(computeShaderFilePath);
		attachShaderSource(program, GL_COMPUTE_SHADER, contents.c_str());
	}
	glLinkProgram(program);
	return std::auto_ptr<IShader>(new Shader(program));
}

std::auto_ptr<ITexture> GLGraphicsProvider::createTextureFromPixmap(const Pixmap& pixmap, const ITextureParameters& parameters)
{
	return std::auto_ptr<ITexture>(new Texture(pixmap));
}

std::auto_ptr<IVertexBuffer> GLGraphicsProvider::createVertexBuffer()
{	
	return std::auto_ptr<IVertexBuffer>(new VertexBuffer());
}

std::auto_ptr<IIndiceBuffer> GLGraphicsProvider::createIndiceBuffer(const IIndiceBufferParameters& parameters)
{
	return std::auto_ptr<IIndiceBuffer>(new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER));
}

std::auto_ptr<IConstantBuffer> GLGraphicsProvider::createConstantBuffer(std::auto_ptr<IShader>& shader, unsigned int bufferIndex, const char* bufferName, const IConstantBufferParameters& parameters)
{
	return std::auto_ptr<IConstantBuffer>(new ConstantBuffer(shader->getID(), bufferIndex, bufferName));
}

std::auto_ptr<IStateBuffer> GLGraphicsProvider::createStateBuffer()
{
	return std::auto_ptr<IStateBuffer>(new StateBuffer());
}