#include "ShaderManager.h"

#include <iostream>
#include <stdio.h>

#define LOCAL_FILE_DIR ""
#define GLOBAL_FILE_DIR ""

GLuint ShaderManager::createShaderProgram(const std::string& vertexShaderFileName, const std::string &fragmentShaderFileName)
{
	//Read contents from first file
	std::string vertexFilename = findFileOrThrow(vertexShaderFileName);
	std::ifstream vertexFile(vertexFilename);
	std::stringstream vertexData;
	vertexData << vertexFile.rdbuf();
	vertexFile.close();
	const std::string& vertexString(vertexData.str());

	//Read contents from second file.
	std::string fragmentFilename = findFileOrThrow(fragmentShaderFileName);
	std::ifstream fragmentFile(fragmentFilename);
	std::stringstream fragmentData;
	fragmentData << fragmentFile.rdbuf();
	fragmentFile.close();
	const std::string& fragmentString(fragmentData.str());

	//std::cout << "\nfragment: \n" << fragmentString << "\n\nvertex: \n" << vertexString << std::endl;
	GLuint program = glCreateProgram();
	attachShaderSource(program, GL_VERTEX_SHADER, vertexString.c_str());
	attachShaderSource(program, GL_FRAGMENT_SHADER, fragmentString.c_str());
	glLinkProgram(program);

	return program;
}

void ShaderManager::attachShaderSource(GLuint prog, GLenum type, const char * source)
{
    GLuint sh;

    sh = glCreateShader(type);
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
		std::cout << "Shader log: " << typeString << " shader: " << buffer << std::endl;
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