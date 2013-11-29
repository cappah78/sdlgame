#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "..\Mesh.h" 
#include "..\CubeMap.h"

typedef unsigned int GLuint;
typedef int GLint;

class CubeMap;
class Camera;

class SkyBox {
public:
	SkyBox(float radius);
	~SkyBox();
	void render(const Camera& camera);
private:
	glm::mat4 m_scale;
	glm::mat4 m_translation;

	GLuint m_skyBoxShader;
	GLint m_mvpLoc;

	CubeMap m_cubeMap;
	Mesh m_mesh;
};

#endif //SKYBOX_H_