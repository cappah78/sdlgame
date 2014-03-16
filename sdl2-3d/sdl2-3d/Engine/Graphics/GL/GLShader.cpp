#include "GLShader.h"

#include "GLCommandQueue.h"
#include "../../Utils/Functor.h"
#include "../../Utils/ShaderManager.h"

bool GLShader::s_begun = false;

GLShader::GLShader() 
{

}

GLShader::GLShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
	: m_begun(false)
{
	setupProgram(vertexShaderPath, geomShaderPath, fragShaderPath);
	glqCheckGLError();
}

GLShader::GLShader(GLuint shaderID)
	: m_begun(false)
	, m_shaderID(shaderID)
{
	glqCheckGLError();
}

GLShader::~GLShader()
{
	glqDeleteProgram(m_shaderID);
}

void GLShader::setupProgram(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath)
{
	glqAddFunctor(Func::makeFunctor(m_shaderID, ShaderManager::createShaderProgram, vertexShaderPath, geomShaderPath, fragShaderPath));
}

void GLShader::begin()
{
	assert(!s_begun);
	assert(!m_begun);
	s_begun = true;
	m_begun = true;
	glqUseProgram(m_shaderID);
}

void GLShader::end()
{
	assert(s_begun);
	assert(m_begun);
	s_begun = false;
	m_begun = false;
	//glqUseProgram(0);	//optional
}

unsigned int GLShader::getID()
{
	return m_shaderID;
}

void GLShader::setUniform1i(const char* const uniformName, int val)
{
	assert(m_begun);

	bool contained = false;
	for (auto it = m_uniformLocs.begin(); it != m_uniformLocs.end(); ++it)
	{
		if (!std::strcmp(it->first, uniformName))
		{
			contained = true;
			glqUniform1i(it->second, val);
		}
	}

	if (!contained)
	{
		m_uniformLocs.emplace_back(uniformName, 0);
		glqGetUniformLocation(m_shaderID, uniformName, m_uniformLocs[m_uniformLocs.size() - 1].second);
		glqUniform1i(m_uniformLocs[m_uniformLocs.size() - 1].second, val);
	}
}
void GLShader::setUniform1f(const char* const uniformName, float val)
{
	assert(m_begun);

	bool contained = false;
	for (auto it = m_uniformLocs.begin(); it != m_uniformLocs.end(); ++it)
	{
		if (!std::strcmp(it->first, uniformName))
		{
			contained = true;
			glqUniform1f(it->second, val);
		}
	}

	if (!contained)
	{
		m_uniformLocs.emplace_back(uniformName, 0);
		glqGetUniformLocation(m_shaderID, uniformName, m_uniformLocs[m_uniformLocs.size() - 1].second);
		glqUniform1f(m_uniformLocs[m_uniformLocs.size() - 1].second, val);
	}
}
void GLShader::setUniform2f(const char* const uniformName, const glm::vec2& vec)
{
	assert(m_begun);

	bool contained = false;
	for (auto it = m_uniformLocs.begin(); it != m_uniformLocs.end(); ++it)
	{
		if (!std::strcmp(it->first, uniformName))
		{
			contained = true;
			glqUniform2f(it->second, vec.x, vec.y);
		}
	}

	if (!contained)
	{
		m_uniformLocs.emplace_back(uniformName, 0);
		glqGetUniformLocation(m_shaderID, uniformName, m_uniformLocs[m_uniformLocs.size() - 1].second);
		glqUniform2f(m_uniformLocs[m_uniformLocs.size() - 1].second, vec.x, vec.y);
	}
}
void GLShader::setUniform3f(const char* const uniformName, const glm::vec3& vec)
{
	assert(m_begun);

	bool contained = false;
	for (auto it = m_uniformLocs.begin(); it != m_uniformLocs.end(); ++it)
	{
		if (!std::strcmp(it->first, uniformName))
		{
			contained = true;
			glqUniform3f(it->second, vec.x, vec.y, vec.z);
		}
	}

	if (!contained)
	{
		m_uniformLocs.emplace_back(uniformName, 0);
		glqGetUniformLocation(m_shaderID, uniformName, m_uniformLocs[m_uniformLocs.size() - 1].second);
		glqUniform3f(m_uniformLocs[m_uniformLocs.size() - 1].second, vec.x, vec.y, vec.z);
	}
}
void GLShader::setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat)
{
	assert(m_begun);

	bool contained = false;
	for (auto it = m_uniformLocs.begin(); it != m_uniformLocs.end(); ++it)
	{
		if (!std::strcmp(it->first, uniformName))
		{
			contained = true;
			glqUniformMatrix4fv(it->second, 1, GL_FALSE, &mat[0][0]);
		}
	}

	if (!contained)
	{
		m_uniformLocs.emplace_back(uniformName, 0);
		glqGetUniformLocation(m_shaderID, uniformName, m_uniformLocs[m_uniformLocs.size() - 1].second);
		glqUniformMatrix4fv(m_uniformLocs[m_uniformLocs.size() - 1].second, 1, GL_FALSE, &mat[0][0]);
	}
}