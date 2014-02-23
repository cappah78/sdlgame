#include "GLShader.h"

#include "../Model/IShader.h"

#include "../../Utils/ShaderManager.h"
#include "../../Utils/CheckGLError.h"

bool GLShader::s_begun = false;


GLShader::GLShader() 
{

}

GLShader::GLShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
	: m_begun(false)
{
	setupProgram(vertexShaderPath, geomShaderPath, fragShaderPath);
	CHECK_GL_ERROR();
}

GLShader::GLShader(GLuint shaderID)
	: m_begun(false)
	, m_shaderID(shaderID)
{
	CHECK_GL_ERROR();
}

GLShader::~GLShader()
{
	glDeleteProgram(m_shaderID);
}

void GLShader::setupProgram(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
{
	m_shaderID = ShaderManager::createShaderProgram(vertexShaderPath, geomShaderPath, fragShaderPath);
}

void GLShader::begin()
{
	assert(!s_begun);
	assert(!m_begun);
	s_begun = true;
	m_begun = true;
	glUseProgram(m_shaderID);
}

void GLShader::end()
{
	assert(s_begun);
	assert(m_begun);
	s_begun = false;
	m_begun = false;
	glUseProgram(0);	//optional
}

unsigned int GLShader::getID()
{
	return m_shaderID;
}

void GLShader::setUniform1i(const char* const uniformName, int val)
{
	assert(m_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLint loc = glGetUniformLocation(m_shaderID, uniformName);
#ifdef SHADER_STRICT_UNIFORM_LOC
		assert(loc != -1);
#endif //SHADER_STRICT_UNIFORM_LOC
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform1i(loc, val);
	}
	else
		glUniform1i(it->second, val);
}
void GLShader::setUniform1f(const char* const uniformName, float val)
{
	assert(m_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLint loc = glGetUniformLocation(m_shaderID, uniformName);
#ifdef SHADER_STRICT_UNIFORM_LOC
		assert(loc != -1);
#endif //SHADER_STRICT_UNIFORM_LOC
		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform1f(loc, val);
	}
	else
		glUniform1f(it->second, val);
}
void GLShader::setUniform2f(const char* const uniformName, const glm::vec2& vec)
{
	assert(m_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLint loc = glGetUniformLocation(m_shaderID, uniformName);
#ifdef SHADER_STRICT_UNIFORM_LOC
		assert(loc != -1);
#endif //SHADER_STRICT_UNIFORM_LOC		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform2f(loc, vec.x, vec.y);
	}
	else
		glUniform2f(it->second, vec.x, vec.y);
}
void GLShader::setUniform3f(const char* const uniformName, const glm::vec3& vec)
{
	assert(m_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLint loc = glGetUniformLocation(m_shaderID, uniformName);
#ifdef SHADER_STRICT_UNIFORM_LOC
		assert(loc != -1);
#endif //SHADER_STRICT_UNIFORM_LOC		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
	else
		glUniform3f(it->second, vec.x, vec.y, vec.z);
}
void GLShader::setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat)
{
	assert(m_begun);
	auto it = m_uniformLocMap.find(uniformName);
	if (it == m_uniformLocMap.end())
	{
		GLint loc = glGetUniformLocation(m_shaderID, uniformName);
#ifdef SHADER_STRICT_UNIFORM_LOC
		assert(loc != -1);
#endif //SHADER_STRICT_UNIFORM_LOC		m_uniformLocMap.insert(std::make_pair(uniformName, loc));
		glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	}
	else
		glUniformMatrix4fv(it->second, 1, GL_FALSE, &mat[0][0]);
}