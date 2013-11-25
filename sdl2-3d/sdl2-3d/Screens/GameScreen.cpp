﻿#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

//#include "VoxelBatch.h"
#include "..\Voxel\VoxelCache.h"
#include "..\Engine\Graphics\Texture.h"
#include "..\Engine\Graphics\TextureRegion.h"
#include "..\Engine\Graphics\Material.h"
#include "..\Engine\Graphics\TextureArray.h"

#include "..\Voxel\StoneBlock.h"
#include "..\Voxel\DirtBlock.h"

#include "../Game.h"

std::vector<VoxelCache::Cache*> caches;
TextureArray* tileSet;

GameScreen::GameScreen()
	: m_camera(glm::vec3(0, 0, 0))
	, m_cameraController(m_camera)
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_DEPTH_TEST);

	StoneBlock block;
	StoneBlock block2;
	DirtBlock block3;

	std::cout << "size: " << sizeof(StoneBlock) << std::endl;

	GLint maxTexLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
	std::cout << "Max texture layers: " << maxTexLayers << std::endl;

	std::vector<const char*> images;
	images.push_back("Assets/Textures/MinecraftBlocks/stone.png");
	images.push_back("Assets/Textures/MinecraftBlocks/dirt.png");
	images.push_back("Assets/Textures/MinecraftBlocks/cobblestone.png");
	images.shrink_to_fit();

	tileSet = new TextureArray(images, 16, 16);
	tileSet->bind();

	const unsigned int chunkSize = 16;

	m_voxelCache = new VoxelCache(chunkSize * chunkSize * chunkSize);
	int y = 0;
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k < 1; ++k) {
				m_voxelCache->beginCache(VoxelCache::TOP, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 1, VoxelCache::AO_UP);
				caches.push_back(m_voxelCache->endCache());

				m_voxelCache->beginCache(VoxelCache::BOTTOM, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 0, VoxelCache::AO_RIGHT);
				caches.push_back(m_voxelCache->endCache());

				m_voxelCache->beginCache(VoxelCache::LEFT, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));;
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 2, 0);
				caches.push_back(m_voxelCache->endCache());

				m_voxelCache->beginCache(VoxelCache::RIGHT, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 2, VoxelCache::AO_DOWN);
				caches.push_back(m_voxelCache->endCache());

				m_voxelCache->beginCache(VoxelCache::FRONT, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 0, VoxelCache::AO_UP);
				caches.push_back(m_voxelCache->endCache());

				m_voxelCache->beginCache(VoxelCache::BACK, float(i * chunkSize), float(k * chunkSize), float(j * chunkSize));
				for (int x = 0; x < chunkSize; x += 2)
				for (int y = 0; y < chunkSize; y += 2)
				for (int z = 0; z < chunkSize; z += 2)
					m_voxelCache->addFace(x, y, z, 0, VoxelCache::AO_LEFT);
				caches.push_back(m_voxelCache->endCache());
			}
		}
	}
	caches.shrink_to_fit();
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();
	m_skyBox.render(m_camera);

	m_voxelCache->beginRender();
	for (VoxelCache::Cache* cache : caches) 
		m_voxelCache->renderCache(cache, tileSet, m_camera);
	m_voxelCache->finishRender();

	Game::graphics.swap();
}

GameScreen::~GameScreen() 
{
	//delete m_voxelBatch;
	for (VoxelCache::Cache* c : caches)
		m_voxelCache->deleteCache(c);
	delete m_voxelCache;
	//delete m_texture;
	//delete m_region;
}



void GameScreen::resize(int width, int height) 
{
	glViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

bool GameScreen::keyDown(SDL_Keysym key)
{
	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

