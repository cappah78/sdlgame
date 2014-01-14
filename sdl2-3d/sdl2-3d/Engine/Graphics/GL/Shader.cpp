#include "Shader.h"

#include "../../Utils/ShaderManager.h"

bool Shader::s_begun = false;

Shader::Shader() 
{

}

Shader::Shader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
{
	setupProgram(vertexShaderPath, geomShaderPath, fragShaderPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderID);
}

void Shader::setupProgram(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
{
	m_shaderID = ShaderManager::createShaderProgram(vertexShaderPath, geomShaderPath, fragShaderPath);
}

void Shader::begin()
{
	assert(!s_begun);
	s_begun = true;
	glUseProgram(m_shaderID);
}
void Shader::end()
{
	assert(s_begun);
	s_begun = false;
	//glUseProgram(0);	//optional
}

void Shader::setUniform1f(const char* const uniformName, float val)
{
	assert(s_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLuint loc = glGetUniformLocation(m_shaderID, uniformName);
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform1f(loc, val);
	}
	else
		glUniform1f(it->second, val);
}
void Shader::setUniform2f(const char* const uniformName, const glm::vec2& vec)
{
	assert(s_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLuint loc = glGetUniformLocation(m_shaderID, uniformName);		
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform2f(loc, vec.x, vec.y);
	}
	else
		glUniform2f(it->second, vec.x, vec.y);
}
void Shader::setUniform3f(const char* const uniformName, const glm::vec3& vec)
{
	assert(s_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLuint loc = glGetUniformLocation(m_shaderID, uniformName);
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
	else
		glUniform3f(it->second, vec.x, vec.y, vec.z);
}
void Shader::setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat)
{
	assert(s_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLuint loc = glGetUniformLocation(m_shaderID, uniformName);
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	}
	else
		glUniformMatrix4fv(it->second, 1, GL_FALSE, &mat[0][0]);
}