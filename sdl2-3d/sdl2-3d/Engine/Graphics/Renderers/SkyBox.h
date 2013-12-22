#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "..\Mesh.h" 
#include "..\CubeMap.h"

typedef unsigned int GLuint;
typedef int GLint;

class CubeMap;
class Camera;

/** 
Skybox renderer which renders a cube map texture(hardcoded) on a cube mesh.
*/
class SkyBox 
{
public:
	/** 
	- float radius : distance from center of cube(camera pos) to corners,
	make sure this is within camera.far but not close enough that it cuts away
	too much geometry
	*/
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