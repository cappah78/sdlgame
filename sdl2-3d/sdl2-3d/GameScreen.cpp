#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>

#include "VoxelBatch.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Material.h"

GameScreen::GameScreen() 
	: m_camera(glm::vec3(0, 0, 0))
	, m_cameraController(m_camera)
{
	SDLGame::registerKeyListener(&m_cameraController);
	SDLGame::registerMouseListener(&m_cameraController);
	SDLGame::registerKeyListener(this);

	m_texture = new Texture("arrow.png");
	m_region = new TextureRegion(m_texture);
	m_material.setDiffuse(m_region);

	m_voxelBatch = new VoxelBatch();

	glEnable(GL_DEPTH_TEST);
}

GameScreen::~GameScreen() 
{
	delete m_texture;
	delete m_region;
}

void renderCube(VoxelBatch* batch, Material& material, int x, int y, int z) {
	batch->renderFace(VoxelBatch::TOP, x, y, z, material);
	batch->renderFace(VoxelBatch::BOTTOM, x, y, z, material);
	batch->renderFace(VoxelBatch::LEFT, x, y, z, material);
	batch->renderFace(VoxelBatch::RIGHT, x, y, z, material);
	batch->renderFace(VoxelBatch::FRONT, x, y, z, material);
	batch->renderFace(VoxelBatch::BACK, x, y, z, material);
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_skyBox.render(m_camera);

	m_voxelBatch->begin(m_camera);
	for (int x = 0; x < 100; ++x) {
		for (int z = 0; z < 10; ++z) {
			renderCube(m_voxelBatch, m_material, x, 0, z);
		}
	}
	m_voxelBatch->end();

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

