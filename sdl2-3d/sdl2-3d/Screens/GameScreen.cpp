#include "GameScreen.h"

#include <glm\glm.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "../Voxel/VoxelRenderer.h"
#include "../Engine/Graphics/GL/TextureArray.h"

#include "../Game.h"
#include "../Engine/Graphics/Color8888.h"

#include "../Engine/Graphics/GL/Mesh.h"

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;

static const float CAMERA_FAR = 280.0f;	//is also fog/chunk load distance

static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 2, -10);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(0, 0, 1);

GameScreen::GameScreen()
	: m_camera(CAMERA_SPAWN_POS,
	CAMERA_SPAWN_DIR,
	float(Game::graphics.getScreenWidth()),
	float(Game::graphics.getScreenHeight()),
	CAMERA_VERTICAL_FOV,
	CAMERA_NEAR,
	CAMERA_FAR)
	, m_cameraController(m_camera, CAMERA_SPAWN_DIR)
	, m_textureManager()
	, m_world(m_textureManager)
	, m_modelShader("Assets/Shaders/modelshader.vert", NULL, "Assets/Shaders/modelshader.frag")
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_DEPTH_TEST);

	/* initialize crytek sponza model rendering /*
	m_modelShader.begin();
	m_modelShader.setUniform1i("u_diffuseTex", 0);
	m_modelShader.setUniform1i("u_normalTex", 1);
	m_modelShader.setUniform1i("u_specularTex", 2);
	m_modelShader.setUniform1i("u_opacityTex", 3);
	m_modelShader.end();

	m_mesh.loadMesh("Assets/Models/crysponza_bubbles/sponza.obj", m_textureManager);
	std::shared_ptr<Mesh::ShaderAttributes> attribs(new Mesh::ShaderAttributes());
	m_mesh.setShaderAttributes(attribs);
	*/
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_world.update(deltaSec, m_camera);
	m_worldRenderer.render(m_world, m_camera);

	//m_deferredWorldRenderer.render(m_world, m_camera);	// wip renderer

	/* renders crytek sponza model /*
	m_modelShader.begin();
	m_modelShader.setUniformMatrix4f("u_mvp", m_camera.m_combinedMatrix);
	m_modelShader.setUniformMatrix4f("u_transform", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 0, -20)), glm::vec3(0.1f, 0.1f, 0.1f)));
	m_mesh.render();
	m_modelShader.end();
	*/

	Game::graphics.swap();
}

GameScreen::~GameScreen() 
{

}

void GameScreen::resize(int width, int height) 
{
	glViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

static int camSpeedTicks = 0;
bool GameScreen::keyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_r)
	{
		//just some quick way to control speed
		camSpeedTicks++;
		m_cameraController.setCameraSpeed(10.0f * ((camSpeedTicks % 7) + 1));
	}

	if (key.sym == SDLK_t)
	{
		m_deferredWorldRenderer.doLight(m_camera);
	}

	if (key.sym == SDLK_f)
	{
		//m_world.doBlockUpdate(glm::ivec3(glm::round(m_camera.m_position)));
	}

	if (key.sym == SDLK_PERIOD)
	{
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far + 10.0f);
	}

	if (key.sym == SDLK_COMMA)
	{
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far - 10.0f);
	}

	if (key.sym == SDLK_F1)
	{
		printf("Controls: \nWASD, Shift, Space to fly \nR to toggle different speeds \nPeriod to increase render distance \nComma to decrease render distance \nF to trigger a block update at location \nT to place a light at location(wip)\n");
	}


	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

