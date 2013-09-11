#include "Shader.h"
#include "ShaderManager.h"

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"

const int Shader::IN_POSITION_LOCATION = 0;
const int Shader::IN_TEXCOORD_LOCATION = 1;
const int Shader::IN_NORMAL_LOCATION = 2;

const char* Shader::U_MVP_NAME = "u_mvp";
const char* Shader::U_MV_NAME = "u_mv";


Shader::Shader(const std::string& vertexShaderFileName, const std::string &fragmentShaderFileName)
{
	shaderId = ShaderManager::createShaderProgram(vertexShaderFileName, fragmentShaderFileName);
	u_mvp = glGetUniformLocation(shaderId, Shader::U_MVP_NAME);
	u_mv = glGetUniformLocation(shaderId, Shader::U_MV_NAME);
}

Shader::~Shader()
{

}

void Shader::use(const Camera& camera)
{
	glUseProgram(shaderId);
	glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &camera.combinedMatrix[0][0]);
	glUniformMatrix4fv(u_mv, 1, GL_FALSE, &camera.viewMatrix[0][0]);
}