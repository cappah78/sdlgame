#ifndef SHADER_H_
#define SHADER_H_

typedef unsigned int GLuint;
class Camera;

class Shader
{
public:
	Shader(const char* vertexShaderFileName, const char* fragmentShaderFileName);
	~Shader();

	static const int IN_POSITION_LOC;
	static const int IN_TEXCOORD_LOC;
	static const int IN_NORMAL_LOC;
	static const char* MVP_LOC;
	static const char* MV_LOC;

	GLuint m_shaderId;

	GLuint m_mvpLoc;
	GLuint m_mvLoc;

	void use(const Camera& camera);
private:

};

#endif //SHADER_H_