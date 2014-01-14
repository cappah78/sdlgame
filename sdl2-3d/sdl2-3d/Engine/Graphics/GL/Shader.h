#ifndef SHADER_H_
#define SHADER_H_

#include <glm\glm.hpp>
#include <string>
#include <map>
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

	void setUniform1f(const char* const uniformName, float val);
	void setUniform2f(const char* const uniformName, const glm::vec2& vec);
	void setUniform3f(const char* const uniformName, const glm::vec3& vec);
	void setUniformMatrix4f(const char* const uniformName, const glm::mat4& mat);

private:
	static bool s_begun;	// global state to avoid mixing multiple shaders.
	GLuint m_shaderID;

	struct StrCmp
	{
		bool operator()(char const *a, char const *b)
		{
			return std::strcmp(a, b) < 0;
		}
	};

	std::map<const char*, GLuint, StrCmp> m_uniformLocMap;
};

#endif //SHADER_H_