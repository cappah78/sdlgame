#include "SkyBox.h"

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include "..\GL\CubeMap.h"
#include "..\..\Utils\ShaderManager.h"
#include "../Camera.h"

static const char* SKYBOX_FACE_NAMES[] = { 
	"Assets/Models/Skybox/right.bmp", 
	"Assets/Models/Skybox/left.bmp", 
	"Assets/Models/Skybox/top.bmp", 
	"Assets/Models/Skybox/bottom.bmp", 
	"Assets/Models/Skybox/front.bmp", 
	"Assets/Models/Skybox/back.bmp" 
};

static const float CORNER_RADIUS_MULTIPLIER = 0.57f; // 2.0f - sqrt(1 * 1 + 1 * 1)

static const char* SKYBOX_MODEL_PATH = "Assets/Models/Skybox/skybox.obj";
static const char* SKYBOX_VERTEX_SHADER_PATH = "Assets/Shaders/skybox.vert";
static const char* SKYBOX_FRAGMENT_SHADER_PATH = "Assets/Shaders/skybox.frag";

SkyBox::SkyBox(float radius)
	: m_mesh(SKYBOX_MODEL_PATH)
	, m_transform(1)
	, m_scale(glm::scale(radius * CORNER_RADIUS_MULTIPLIER, radius * CORNER_RADIUS_MULTIPLIER, radius * CORNER_RADIUS_MULTIPLIER))
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
	// translate
	m_transform[3][0] = camera.m_position.x; //to negate camera movement relative to skybox
	m_transform[3][1] = camera.m_position.y;
	m_transform[3][2] = camera.m_position.z;
	
	glm::mat4 mvp = camera.m_combinedMatrix * m_transform * m_scale;

	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	m_cubeMap.bind(GL_TEXTURE0);
	//m_mesh.render();//TODO: fixme

	glUseProgram(0);
}

void SkyBox::setToRotation(float angle, const glm::vec3& axis)
{
	m_transform = glm::rotate(angle, axis);
}