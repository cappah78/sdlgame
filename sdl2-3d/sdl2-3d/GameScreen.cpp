#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>

#include "Texture.h"
#include "Terrain.h"
#include "ShaderManager.h"

GameScreen::GameScreen()
	: m_mouseLookSystem(glm::vec3(0.0001, 0, -1.0))
	, m_camera(glm::vec3(0, 50, 0))
	, m_cameraController(m_camera)
	, m_heightMap(23, 512, 512)
{
	SDLGame::registerKeyListener(&m_cameraController);
	SDLGame::registerMouseListener(&m_mouseLookSystem);
	SDLGame::registerKeyListener(this);
	
	m_heightMap.addPerlinNoise(30.0f);
	m_heightMap.addPerlinNoise(20.0f);
	m_heightMap.addPerlinNoise(10.0f);
	m_heightMap.addPerlinNoise(5.0f);
	m_heightMap.addPerlinNoise(1.0f);

	m_heightMap.perturb(250.0f, 8.0f);
	for(int i = 0; i < 5; i++)
		m_heightMap.erode(16.0f);
	m_heightMap.smoothen();
	m_heightMap.smoothen();


	m_terrain = new Terrain(m_heightMap, 10, 150);
	m_texture = new Texture("grass.png");

	m_world.registerSystem(m_movementSystem);
	m_world.registerSystem(m_mouseLookSystem);
	m_world.registerSystem(m_cameraController);

	//EntityFactory::createPlayer(world, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
}

GameScreen::~GameScreen() 
{
	delete m_texture;
	delete m_terrain;
}

bool updateLights = true;

void GameScreen::render(float deltaSec)
{
	//std::cout << "drawan" << std::endl;
	m_world.loopStart();
	m_world.setDelta(deltaSec);
	m_world.update();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.lookAtDir(m_mouseLookSystem.getLookDir());
	m_camera.update();
	
	m_skyBox.render(m_camera);

	if (updateLights) {
		updateLights = false;
		forwardShader.updateLights(m_camera, m_lightManager);
		forwardShader.generateShadowMaps();
		m_terrain->render();
		forwardShader.finishShadowMaps();
	}

	forwardShader.use(m_camera);
	m_texture->bind();
	m_terrain->render();

	SDLGame::swap();
}

void GameScreen::resize(int width, int height) 
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

bool GameScreen::keyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_r)
	{
		updateLights = true;
	}
	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

