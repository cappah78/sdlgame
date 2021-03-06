#pragma once
#if 0	// depricated for now

#include "..\GL\GLMesh.h" 
#include "..\GL\GLCubeMap.h"

typedef unsigned int GLuint;
typedef int GLint;

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
	void setToRotation(float angle, const glm::vec3& axis);
private:
	glm::mat4 m_scale;
	glm::mat4 m_transform;

	GLuint m_skyBoxShader;
	GLint m_mvpLoc;

	GLCubeMap m_cubeMap;
	GLMesh m_mesh;
};
#endif