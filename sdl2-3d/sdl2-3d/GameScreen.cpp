#include "GameScreen.h"

#include <iostream>
#include <stdio.h>

#define TERRAIN_VERTEX_SHADER "terrain.vert"
#define TERRAIN_FRAGMENT_SHADER "terrain.frag"

GameScreen::GameScreen() 
	: batch(4)
	, camera(glm::vec3(0, 10, 0), glm::vec3(0, 0, -1))
	, cameraController(camera)
	, heightMap(23, 32, 32)
	, terrainShader(TERRAIN_VERTEX_SHADER, TERRAIN_FRAGMENT_SHADER)
	, lightManager(terrainShader.shaderId)
{
	/*
	std::cout << "init" << std::endl;
	SDLGame::registerKeyListener(&cameraController);
	SDLGame::registerMouseListener(&mouseLookSystem);
	std::cout << "Registered input listeners" << std::endl;

	heightMap.addPerlinNoise(30.0f);
	heightMap.addPerlinNoise(10.0f);
	heightMap.addPerlinNoise(5.0f);
	heightMap.addPerlinNoise(5.0f);
	heightMap.perturb(500.0f, 20.0f);

	for(int i = 0; i < 20; i++)
		heightMap.erode(128.0f);
	heightMap.smoothen();
	heightMap.smoothen();
	std::cout << "Created heightmap" << std::endl;

	terrain = new Terrain2(heightMap, 5);
	SDLGame::registerKeyListener(terrain);
	std::cout << "Created terrain" << std::endl;

	texture = new Texture("grass.png");
	texture->bind();
	std::cout << "Created texture" << std::endl;

	world.registerSystem(movementSystem);
	world.registerSystem(mouseLookSystem);

	EntityFactory::createPlayer(world, 1, 1, 0);
	*/
}

GameScreen::~GameScreen() 
{
	delete texture;
	delete texture2;
	delete texture3;
	delete region;

	delete terrain;
}

void GameScreen::render(float deltaSec) 
{
	std::cout << "Renderin:" << std::endl;
	/*
	world.loopStart();
	world.setDelta(deltaSec);
	world.update();
	*/
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	cameraController.update(deltaSec);
	camera.lookAtDir(mouseLookSystem.getLookDir());

	//camera.position.y = terrain->getHeightAt(camera.position) + 10.0f;
	
	camera.update();
	terrainShader.begin(camera);
	lightManager.update(camera);
	terrain->render();
	terrainShader.end();
	*/
	SDLGame::swap();
}

void GameScreen::resize(int width, int height) 
{
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}

