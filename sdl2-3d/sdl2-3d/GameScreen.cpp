﻿#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

//#include "VoxelBatch.h"
#include "VoxelCache.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Material.h"
#include "TextureArray.h"
#include "GameLoop.h"

std::vector<VoxelCache::Cache*> caches;
VoxelCache::Cache* da;
TextureArray* arr;

GameScreen::GameScreen()
: m_camera(glm::vec3(0, 0, 0))
, m_cameraController(m_camera)
{
	GameLoop::registerKeyListener(&m_cameraController);
	GameLoop::registerMouseListener(&m_cameraController);
	GameLoop::registerKeyListener(this);
	glEnable(GL_DEPTH_TEST);

	GLint maxTexLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
	std::cout << "Max texture layers: " << maxTexLayers << std::endl;

	std::vector<const char*> images;
	images.push_back("blocks/stone.png");
	images.push_back("blocks/dirt.png");
	images.push_back("blocks/cobblestone.png");
	arr = new TextureArray(images, 16, 16);

	const unsigned int chunkSize = 16;

	m_voxelCache = new VoxelCache(chunkSize * chunkSize * chunkSize);

	for (int i = 0; i < 20; ++i) {
		m_voxelCache->beginCache(VoxelCache::TOP, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 1, VoxelCache::AO_UP);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::BOTTOM, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 0, 0);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::LEFT, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 2, VoxelCache::AO_DOWN);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::RIGHT, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 0, 0);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::FRONT, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 0, 0);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::BACK, float(i * chunkSize), 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, 0, 0);
		caches.push_back(m_voxelCache->endCache());
	}
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

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_skyBox.render(m_camera);

	for (VoxelCache::Cache* c : caches)
		m_voxelCache->renderCache(c, arr, m_camera);

	GameLoop::swap();
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

