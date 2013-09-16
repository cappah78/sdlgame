#include "ShaderManager.h"

#include <gl/glew.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

#define LOCAL_FILE_DIR ""
#define GLOBAL_FILE_DIR ""

GLuint ShaderManager::createShaderProgram(const char* vertexShaderFileName, const char* geometryShaderFileName, const char* fragmentShaderFileName)
{
	const std::string* vertexFileData = getStringFromFile(vertexShaderFileName);
	const std::string* geometryFileData = getStringFromFile(geometryShaderFileName);
	const std::string* fragmentFileData = getStringFromFile(fragmentShaderFileName);

	GLuint program = glCreateProgram();
	if (vertexFileData) attachShaderSource(program, GL_VERTEX_SHADER, vertexFileData->c_str());
	if (geometryFileData) attachShaderSource(program, GL_GEOMETRY_SHADER_EXT, geometryFileData->c_str());
	if (fragmentFileData) attachShaderSource(program, GL_FRAGMENT_SHADER, fragmentFileData->c_str());
	glLinkProgram(program);

	delete vertexFileData;
	delete geometryFileData;
	delete fragmentFileData;

	return program;
}

const std::string* ShaderManager::getStringFromFile(const char* fileName)
{
	if (fileName == NULL)
		return 0;

	std::string actualFileName = findFileOrThrow(fileName);
	std::ifstream file(actualFileName);
	std::stringstream data;
	data << file.rdbuf();
	file.close();
	
	const std::string* str = new std::string(data.str());
	return str;
}


void ShaderManager::attachShaderSource(GLuint prog, GLenum type, const char * source)
{
    GLuint sh;

    sh = glCreateShader(type);

	if (sh == 0)
	{
		std::cout << "Could not create shader: " << type << std::endl;
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
		switch(type) {
		case GL_FRAGMENT_SHADER:
			typeString = "fragment";
			break;
		case GL_VERTEX_SHADER:
			typeString = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			typeString = "geometry";
			break;
		default:
			typeString = "nan";
			break;
		}
		std::cout << "Error in " << typeString << " shader: " << buffer << std::endl;
	}

    glAttachShader(prog, sh);
    glDeleteShader(sh);
}

std::string ShaderManager::findFileOrThrow( const std::string &strBasename )
{

	std::string strFilename = LOCAL_FILE_DIR + strBasename;
	std::ifstream testFile(strFilename.c_str());
	if(testFile.is_open())
		return strFilename;

	strFilename = GLOBAL_FILE_DIR + strBasename;
	testFile.open(strFilename.c_str());
	if(testFile.is_open())
		return strFilename;

	throw std::runtime_error("Could not find the file " + strBasename);
}