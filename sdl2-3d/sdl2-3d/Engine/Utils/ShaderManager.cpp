#include "ShaderManager.h"

#include "FileReader.h"

#include <gl/glew.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

#define LOCAL_FILE_DIR ""
#define GLOBAL_FILE_DIR ""

GLuint ShaderManager::createShaderProgram(const char* vertexShaderFilePath, const char* geometryShaderFilePath, const char* fragmentShaderFilePath)
{
	GLuint program = glCreateProgram();

	if (vertexShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(vertexShaderFilePath);
		attachShaderSource(program, GL_VERTEX_SHADER, contents.c_str());
	}
	if (geometryShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(geometryShaderFilePath);
		attachShaderSource(program, GL_GEOMETRY_SHADER, contents.c_str());
	}
	if (fragmentShaderFilePath)
	{
		std::string contents = FileReader::readStringFromFile(fragmentShaderFilePath);
		attachShaderSource(program, GL_FRAGMENT_SHADER, contents.c_str());
	}
	glLinkProgram(program);
	return program;
}

void ShaderManager::attachShaderSource(GLuint prog, GLenum type, const char * source)
{
    GLuint sh;

    sh = glCreateShader(type);

	if (sh == 0)
	{
		std::cerr << "Could not create shader: " << type << std::endl;
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
		std::cerr << "Error in " << typeString << " shader: " << buffer << " : " << source << std::endl;
	}

    glAttachShader(prog, sh);
    glDeleteShader(sh);
}