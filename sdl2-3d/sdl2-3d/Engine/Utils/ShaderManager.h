#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <string>
#include <gl\glew.h>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class ShaderManager
{
public:
	static GLuint createShaderProgram(const char* vertexShaderFileName, const char* geometryShaderFileName, const char* fragmentShaderFileName);
private:
	static void attachShaderSource(GLuint prog, GLenum type, const char * source);
	static const std::string* getStringFromFile(const char* fileName);
	static std::string findFileOrThrow(const std::string &strBasename);
};

#endif //SHADERMANAGER_H_