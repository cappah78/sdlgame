#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>

#include "Texture.h"
#include "Terrain.h"
#include "ShaderManager.h"

#define m_terrainVERTEX_SHADER "terrain.vert"
#define m_terrainFRAGMENT_SHADER "terrain.frag"
#define GEOMETRY_PASS_VERTEX_SHADER "geometrypass.vert"
#define GEOMETRY_PASS_FRAGMENT_SHADER "geometrypass.frag"
#define SKYBOX_VERTEX_SHADER "skybox.vert"
#define SKYBOX_FRAGMENT_SHADER "skybox.frag"

GameScreen::GameScreen()
	: mouseLookSystem_(glm::vec3(0.0001, 0, -1.0))
	, m_camera(glm::vec3(0, 50, 0))
	, cameraController_(m_camera)
	, heightMap_(23, 512, 512)
{
	SDLGame::registerKeyListener(&cameraController_);
	SDLGame::registerMouseListener(&mouseLookSystem_);
	SDLGame::registerKeyListener(this);
	
	heightMap_.addPerlinNoise(30.0f);
	heightMap_.addPerlinNoise(20.0f);
	heightMap_.addPerlinNoise(10.0f);
	heightMap_.addPerlinNoise(5.0f);
	heightMap_.addPerlinNoise(1.0f);

	heightMap_.perturb(250.0f, 8.0f);
	for(int i = 0; i < 5; i++)
		heightMap_.erode(16.0f);
	heightMap_.smoothen();
	heightMap_.smoothen();


	m_terrain = new Terrain(heightMap_, 10, 150);
	m_texture = new Texture("grass.png");

	m_world.registerSystem(movementSystem_);
	m_world.registerSystem(mouseLookSystem_);
	m_world.registerSystem(cameraController_);

	//EntityFactory::createPlayer(world, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
}

GameScreen::~GameScreen() 
{
	delete m_texture;
	delete m_terrain;
}

bool update = true;

void GameScreen::render(float deltaSec)
{
	//std::cout << "drawan" << std::endl;
	m_world.loopStart();
	m_world.setDelta(deltaSec);
	m_world.update();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.lookAtDir(mouseLookSystem_.getLookDir());
	m_camera.update();
	
	m_skyBox.render(m_camera);

	if (update) {
		update = false;
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
		update = true;
	}
	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

