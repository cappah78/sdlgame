#pragma once

typedef unsigned int GLuint;
typedef unsigned int GLenum;

/** Helper class to easily create OpenGL Shaders */
class ShaderManager
{
public:
	/** pass NULL if unused */
	static GLuint createShaderProgram(const char* vertexShaderFileName, const char* geometryShaderFileName, const char* fragmentShaderFileName);
private:
	static void attachShaderSource(GLuint prog, GLenum type, const char * source);
};