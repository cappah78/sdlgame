#include "GameScreen.h"

#include <glm\glm.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "../Voxel/VoxelRenderer.h"
#include "../Engine/Graphics/GL/TextureArray.h"

#include "../Game.h"
#include "../Engine/Graphics/Color8888.h"

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;

static const float CAMERA_FAR = 400.0f;	//is also fog/chunk load distance

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
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_DEPTH_TEST);

#ifdef _DEBUG
	GLint maxTexLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
	std::cout << "Max texture layers: " << maxTexLayers << std::endl;
#endif //_DEBUG
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_world.update(deltaSec, m_camera);
	//m_worldRenderer.render(m_world, m_camera);
	m_deferredWorldRenderer.render(m_world, m_camera);
	//m_skyBox.render(m_camera);

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

bool GameScreen::keyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_r)
	{
		//m_worldRenderer.doLights(m_camera);
		m_deferredWorldRenderer.doLight(m_camera);
	}

	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

