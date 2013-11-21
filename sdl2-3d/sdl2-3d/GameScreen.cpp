#include "GameScreen.h"

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

std::vector<VoxelCache::Cache*> caches;
VoxelCache::Cache* da;

GameScreen::GameScreen()
: m_camera(glm::vec3(0, 0, 0))
, m_cameraController(m_camera)
{
	SDLGame::registerKeyListener(&m_cameraController);
	SDLGame::registerMouseListener(&m_cameraController);
	SDLGame::registerKeyListener(this);
	glEnable(GL_DEPTH_TEST);

	m_texture = new Texture("arrow.png");
	m_region = new TextureRegion(m_texture);
	m_material.setDiffuse(m_region);

	const unsigned int chunkSize = 16;

	m_voxelCache = new VoxelCache(chunkSize * chunkSize * chunkSize);

	for (int i = 0; i < 20; ++i) {
		m_voxelCache->beginCache(VoxelCache::TOP, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::BOTTOM, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::LEFT, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::RIGHT, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::FRONT, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());

		m_voxelCache->beginCache(VoxelCache::BACK, i * chunkSize, 0, 0);
		for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
		for (int z = 0; z < chunkSize; ++z)
			m_voxelCache->addFace(x, y, z, m_material);
		caches.push_back(m_voxelCache->endCache());
	}
}

GameScreen::~GameScreen() 
{
	//delete m_voxelBatch;
	for (VoxelCache::Cache* c : caches)
		m_voxelCache->deleteCache(c);
	delete m_voxelCache;
	delete m_texture;
	delete m_region;
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_skyBox.render(m_camera);

	for (VoxelCache::Cache* c : caches)
		m_voxelCache->renderCache(c, m_camera);

	SDLGame::swap();
}

void GameScreen::resize(int width, int height) 
{
	screenWidth = width;
	screenHeight = height;
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

