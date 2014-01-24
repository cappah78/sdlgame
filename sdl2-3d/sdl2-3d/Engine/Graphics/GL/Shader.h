#ifndef SHADER_H_
#define SHADER_H_

#include <glm\glm.hpp>
#include <unordered_map>
#include <gl\glew.h>

class Shader
{
public:
	Shader();
	Shader(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	virtual ~Shader();
	Shader(const Shader& copy) = delete;

	virtual void setupProgram(const char* const vertexShaderPath, const char* const geomShaderPath, const char* const fragShaderPath);
	virtual void begin();
	virtual void end();

	//TODO: moar uniforms

	void setUniform1i(const char* const uniformName, int val);
	void setUniform1f(const char* const uniformName, float val);
	void setUniform2f(const char* const uniformName, const glm::vec2& vec);
	void setUniform3f(const char* const uniformName, const glm::vec3& vec);
	void setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat);

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

	std::unordered_map<const char*, GLint, StrHash, StrCmp> m_uniformLocMap;
};

#endif //SHADER_H_