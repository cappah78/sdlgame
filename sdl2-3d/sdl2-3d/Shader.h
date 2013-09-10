#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Light.h"


class Shader
{
public:
	Shader(const std::string& vertexShaderFileName, const std::string &fragmentShaderFileName);
	~Shader();

	static const int IN_POSITION_LOCATION;
	static const int IN_TEXCOORD_LOCATION;
	static const int IN_NORMAL_LOCATION;

	static const char* U_MVP_NAME;
	static const char* U_MV_NAME;

	GLuint shaderId;

	GLuint u_mvp;
	GLuint u_mv;

	void use(const Camera& camera);
private:

};

#endif //SHADER_H_