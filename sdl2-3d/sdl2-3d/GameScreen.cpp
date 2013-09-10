﻿#include "GameScreen.h"

#include <iostream>
#include <stdio.h>

#define TERRAIN_VERTEX_SHADER "terrain.vert"
#define TERRAIN_FRAGMENT_SHADER "terrain.frag"

#define GEOMETRY_PASS_VERTEX_SHADER "geometrypass.vert"
#define GEOMETRY_PASS_FRAGMENT_SHADER "geometrypass.frag"

#define SKYBOX_VERTEX_SHADER "skybox.vert"
#define SKYBOX_FRAGMENT_SHADER "skybox.frag"

GameScreen::GameScreen() 
	: camera(glm::vec3(0, 10, 0), glm::vec3(0, 0, -1))
	, cameraController(camera)
	, heightMap(23, 512, 512)
	, terrainShader(TERRAIN_VERTEX_SHADER, TERRAIN_FRAGMENT_SHADER)
	, skyBoxShader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER)
	, lightManager(terrainShader.shaderId)
{
	SDLGame::registerKeyListener(&cameraController);
	SDLGame::registerMouseListener(&mouseLookSystem);
	
	heightMap.addPerlinNoise(30.0f);
	heightMap.addPerlinNoise(20.0f);
	heightMap.addPerlinNoise(10.0f);
	heightMap.addPerlinNoise(5.0f);
	heightMap.addPerlinNoise(1.0f);

	heightMap.perturb(250.0f, 8.0f);
	for(int i = 0; i < 5; i++)
		heightMap.erode(16.0f);
	heightMap.smoothen();
	heightMap.smoothen();


	terrain = new Terrain2(heightMap, 10, 100);
	SDLGame::registerKeyListener(terrain);

	texture = new Texture("grass.png");

	world.registerSystem(movementSystem);
	world.registerSystem(mouseLookSystem);

	//EntityFactory::createPlayer(world, 1, 1, 0);
}

GameScreen::~GameScreen() 
{
	delete texture;
	delete terrain;
}

void GameScreen::render(float deltaSec) 
{
	//std::cout << "drawan" << std::endl;
	world.loopStart();
	world.setDelta(deltaSec);
	world.update();

	glClearColor(0.5, 0.2, 0.2, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraController.update(deltaSec);
	camera.lookAtDir(mouseLookSystem.getLookDir());
	camera.update();
	
	skyBoxShader.use(camera);
	skyBox.render();

	terrainShader.use(camera);
	lightManager.update(camera);

	texture->bind();
	terrain->render();
	
	SDLGame::swap();
}

void GameScreen::resize(int width, int height) 
{
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}

