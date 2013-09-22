#include "SkyBox.h"

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "CubeMap.h"
#include "ShaderManager.h"
#include "Camera.h"


SkyBox::SkyBox()
	: m_mesh("box.obj")
{
	std::string* textureNames = new std::string[6];
	textureNames[0] = "skybox/right.bmp";
	textureNames[1] = "skybox/left.bmp";
	textureNames[2] = "skybox/top.bmp";
	textureNames[3] = "skybox/bottom.bmp";
	textureNames[5] = "skybox/back.bmp";
	textureNames[4] = "skybox/front.bmp";

	m_cubeMap = CubeMap::createFromTextures(textureNames);
	delete [] textureNames;

	m_skyBoxShader = ShaderManager::createShaderProgram("skybox.vert", 0, "skybox.frag");
	m_mvpLoc =  glGetUniformLocation(m_skyBoxShader, "u_mvp");
}

SkyBox::~SkyBox()
{
	delete m_cubeMap;
}

void SkyBox::render(const Camera& camera)
{
	glUseProgram(m_skyBoxShader);

	glm::mat4 scale = glm::scale(30.0f, 30.0f, 30.0f);
	glm::mat4 translation = glm::translate(camera.m_position);
	glm::mat4 mvp = camera.m_combinedMatrix * translation * scale;

	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	m_cubeMap->bind(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);	//TODO: make back facing box mesh
	m_mesh.render();
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}