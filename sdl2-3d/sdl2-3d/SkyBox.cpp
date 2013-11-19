#include "SkyBox.h"

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "CubeMap.h"
#include "ShaderManager.h"
#include "Camera.h"

const char* textureNames[] = { "skybox/right.bmp", "skybox/left.bmp", "skybox/top.bmp", "skybox/bottom.bmp", "skybox/front.bmp", "skybox/back.bmp" };

SkyBox::SkyBox()
	: m_mesh("box.obj")
	, m_cubeMap(&textureNames[0])
{
	m_skyBoxShader = ShaderManager::createShaderProgram("skybox.vert", 0, "skybox.frag");
	m_mvpLoc =  glGetUniformLocation(m_skyBoxShader, "u_mvp");
}

SkyBox::~SkyBox()
{

}

void SkyBox::render(const Camera& camera)
{
	glUseProgram(m_skyBoxShader);

	glm::mat4 scale = glm::scale(30.0f, 30.0f, 30.0f);	//origional is unit cube
	glm::mat4 translation = glm::translate(camera.m_position);
	glm::mat4 mvp = camera.m_combinedMatrix * translation * scale;

	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	m_cubeMap.bind(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);	//TODO: make back facing box mesh

	m_mesh.render();

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}