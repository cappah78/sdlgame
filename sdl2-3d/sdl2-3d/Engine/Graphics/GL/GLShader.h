#pragma once

#include "../Model/IShader.h"

#include <glm\glm.hpp>
#include <unordered_map>
#include <gl\glew.h>

class GLShader : public IShader
{
public:
	GLShader();
	GLShader(GLuint shaderID);
	GLShader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	virtual ~GLShader() override;
	GLShader(const GLShader& copy) = delete;

	void setupProgram(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	virtual void begin() override;
	virtual void end() override;
	virtual unsigned int getID() override;

	//TODO: moar uniforms

	void setUniform1i(const char* const uniformName, int val);
	void setUniform1f(const char* const uniformName, float val);
	void setUniform2f(const char* const uniformName, const glm::vec2& vec);
	void setUniform3f(const char* const uniformName, const glm::vec3& vec);
	void setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat);

	GLuint getShaderID() { return m_shaderID; };	//not const because state can easily be changed with this id

private:
	static bool s_begun;	// global state to avoid mixing multiple shaders.
	bool m_begun;
	GLuint m_shaderID;

	struct StrCmp
	{
		bool operator() (const char* a, const char* b)
		{
			return std::strcmp(a, b) == 0;
		}
	};

	struct StrHash //BKDR hash algorithm
	{	
		int operator() (const char* str) const
		{
			const int seed = 131;
			int hash = 0;
			while (*str)
			{
				hash = (hash * seed) + (*str);
				str++;
			}
			return hash & (0x7FFFFFFF);
		}
	};

	std::vector<std::pair<const char*, GLint>> m_uniformLocs;
	std::unordered_map<const char*, GLint, StrHash, StrCmp> m_uniformLocMap;
};