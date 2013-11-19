#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "Mesh.h" 
#include "CubeMap.h"

typedef unsigned int GLuint;
typedef int GLint;

class CubeMap;
class Camera;

class SkyBox {
public:
	SkyBox();
	~SkyBox();
	void render(const Camera& camera);
private:
	GLuint m_skyBoxShader;
	GLint m_mvpLoc;

	CubeMap m_cubeMap;
	Mesh m_mesh;
};

#endif //SKYBOX_H_