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
static const float CAMERA_FAR = 200.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 2, -10);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(0, 0, 1);

static const int NUM_CHUNKS_X = 10;
static const int NUM_CHUNKS_Y = 10;
static const int NUM_CHUNKS_Z = 10;

GameScreen::GameScreen()
	: m_camera(CAMERA_SPAWN_POS,
		CAMERA_SPAWN_DIR,
		float(Game::graphics.getScreenWidth()), 
		float(Game::graphics.getScreenHeight()), 
		CAMERA_VERTICAL_FOV, 
		CAMERA_NEAR, 
		CAMERA_FAR)
	, m_cameraController(m_camera, CAMERA_SPAWN_DIR)
	//, m_skyBox(CAMERA_FAR)
	, m_textureManager(16, 16)
	, m_world(m_textureManager)
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_DEPTH_TEST);

	GLint maxTexLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
	std::cout << "Max texture layers: " << maxTexLayers << std::endl;
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	//m_world.update(deltaSec);
	m_worldRenderer.render(m_world, m_camera);

	//m_skyBox.render(m_camera);

	Game::graphics.swap();
}

GameScreen::~GameScreen() 
{
	m_chunkRenderData.clear();
}

void GameScreen::resize(int width, int height) 
{
	glViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

bool GameScreen::keyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_r)
		printf("campos: %f, %f, %f \n", m_camera.m_position.x, m_camera.m_position.y, m_camera.m_position.z);
	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

