#include "Shader.h"
#include "ShaderManager.h"

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include "Light.h"

const int Shader::IN_POSITION_LOC = 0;
const int Shader::IN_TEXCOORD_LOC = 1;
const int Shader::IN_NORMAL_LOC = 2;

const char* Shader::MVP_LOC = "u_mvp";
const char* Shader::MV_LOC = "u_mv";


Shader::Shader(const char* vertexShaderFileName, const char* fragmentShaderFileName)
{
	m_shaderId = ShaderManager::createShaderProgram(vertexShaderFileName, 0, fragmentShaderFileName);
	m_mvpLoc = glGetUniformLocation(m_shaderId, Shader::MVP_LOC);
	m_mvLoc = glGetUniformLocation(m_shaderId, Shader::MV_LOC);
}

Shader::~Shader()
{

}

void Shader::use(const Camera& camera)
{
	glm::mat4 scale = glm::scale(30.0f, 30.0f, 30.0f);
	glm::mat4 translation = glm::translate(camera.m_position);
	glm::mat4 da = camera.m_combinedMatrix * translation * scale;
	glUseProgram(m_shaderId);
	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &da[0][0]);
	glUniformMatrix4fv(m_mvLoc, 1, GL_FALSE, &camera.m_viewMatrix[0][0]);
}