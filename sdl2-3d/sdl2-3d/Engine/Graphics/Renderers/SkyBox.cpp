#include "SkyBox.h"

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include "..\CubeMap.h"
#include "..\..\Utils\ShaderManager.h"
#include "..\Camera.h"
#include "../PerspectiveCamera.h"

static const char* SKYBOX_FACE_NAMES[] = { 
	"Assets/Models/Skybox/right.bmp", 
	"Assets/Models/Skybox/left.bmp", 
	"Assets/Models/Skybox/top.bmp", 
	"Assets/Models/Skybox/bottom.bmp", 
	"Assets/Models/Skybox/front.bmp", 
	"Assets/Models/Skybox/back.bmp" 
};

static const char* SKYBOX_MODEL_PATH = "Assets/Models/Skybox/skybox.obj";
static const char* SKYBOX_VERTEX_SHADER_PATH = "Assets/Shaders/skybox.vert";
static const char* SKYBOX_FRAGMENT_SHADER_PATH = "Assets/Shaders/skybox.frag";

static const float SKYBOX_RADIUS = 30.0f;
static const glm::mat4 scale = glm::scale(SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS);	//origional is unit cube (2x2x2)
static glm::mat4 translation = glm::mat4(1);

static float lastCameraFar = 0.0f;

SkyBox::SkyBox()
	: m_mesh(SKYBOX_MODEL_PATH)	//relative from Mesh.cpp
	, m_cubeMap(&SKYBOX_FACE_NAMES[0])
{
	m_skyBoxShader = ShaderManager::createShaderProgram(SKYBOX_VERTEX_SHADER_PATH, 0, SKYBOX_FRAGMENT_SHADER_PATH);
	m_mvpLoc = glGetUniformLocation(m_skyBoxShader, "u_mvp");
}

SkyBox::~SkyBox()
{

}

void SkyBox::render(const Camera& camera)
{

	glUseProgram(m_skyBoxShader);
	translation[3][0] = camera.m_position.x; //to negate camera movement relative to skybox
	translation[3][1] = camera.m_position.y;
	translation[3][2] = camera.m_position.z;
	
	glm::mat4 mvp = camera.m_combinedMatrix * translation * scale;

	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	m_cubeMap.bind(GL_TEXTURE0);
	//glDisable(GL_DEPTH_TEST);

	m_mesh.render();

	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}