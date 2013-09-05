#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <string.h>


#include <gl\glew.h>

class ShaderManager
{
public:
	static GLuint createShaderProgram(const std::string& vertexShaderFileName, const std::string &fragmentShaderFileName);
private:
	static void attachShaderSource(GLuint prog, GLenum type, const char * source);
	static std::string findFileOrThrow(const std::string &strBasename);
};

#endif //SHADERMANAGER_H_