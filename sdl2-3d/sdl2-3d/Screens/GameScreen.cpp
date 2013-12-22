#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "../Voxel/VoxelRenderer.h"
#include "../Engine/Graphics/Texture.h"
#include "../Engine/Graphics/TextureRegion.h"
#include "../Engine/Graphics/Material.h"
#include "../Engine/Graphics/TextureArray.h"

#include "../Voxel/VoxelWorld.h"


#include "../Game.h"
#include "../Engine/Graphics/Color8888.h"

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;
static const float CAMERA_FAR = 300.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 0, 0);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(1, 0, 0);

GameScreen::GameScreen()
	: m_camera(CAMERA_SPAWN_POS,
		CAMERA_SPAWN_DIR,
		float(Game::graphics.getScreenWidth()), 
		float(Game::graphics.getScreenHeight()), 
		CAMERA_VERTICAL_FOV, 
		CAMERA_NEAR, 
		CAMERA_FAR)
	, m_cameraController(m_camera)
	, m_skyBox(CAMERA_FAR)	// its a box, radius is from the middle to the center of a face, so to make sure edges are within far
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	TextureManager textureManager(16, 16);
	VoxelWorld world(textureManager);

	glEnable(GL_DEPTH_TEST);

	GLint maxTexLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
	std::cout << "Max texture layers: " << maxTexLayers << std::endl;

	std::vector<const char*> images;
	images.push_back("Assets/Textures/MinecraftBlocks/stone.png");
	images.push_back("Assets/Textures/MinecraftBlocks/dirt.png");
	images.push_back("Assets/Textures/MinecraftBlocks/cobblestone.png");
	images.push_back("Assets/Textures/MinecraftBlocks/brick.png");
	images.push_back("Assets/Textures/MinecraftBlocks/diamond_block.png");
	images.push_back("Assets/Textures/MinecraftBlocks/planks_oak.png");
	images.push_back("Assets/Textures/MinecraftBlocks/obsidian.png");
	images.push_back("Assets/Textures/MinecraftBlocks/netherrack.png");
	images.push_back("Assets/Textures/MinecraftBlocks/gravel.png");
	images.push_back("Assets/Textures/MinecraftBlocks/sand.png");
	m_tileSet = new TextureArray(images, 16, 16);

	Color8888 color(0, 0, 0, 255);

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 1; ++j) {
			for (int k = 0; k < 15; ++k) {
				for (int face = 0; face < 6; ++face)	//iterate over the 6 different faces
				{
					VoxelRenderer::Cache* cache = m_voxelRenderer.createCache(i * 16.0f, j * 16.0f, k * 16.0f);

					m_voxelRenderer.beginCache(cache);
					for (int x = 0; x < 16; x += 2)
					for (int y = 0; y < 16; y += 2)
					for (int z = 0; z < 16; z += 2)
						cache->addFace((VoxelRenderer::Face) face, x, y, z, (x + y + z + i + j + k) % images.size(), color, color, color, color);
					m_voxelRenderer.endCache(cache);

					m_chunkRenderData.push_back(cache);
				}
			}
		}
	}
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

	m_voxelRenderer.beginRender(m_tileSet);
	for (VoxelRenderer::Cache* cache : m_chunkRenderData)
		m_voxelRenderer.renderCache(cache, m_camera);
	m_voxelRenderer.endRender();

	m_skyBox.render(m_camera);

	Game::graphics.swap();
}

GameScreen::~GameScreen() 
{
	for (VoxelRenderer::Cache* cache : m_chunkRenderData)
		m_voxelRenderer.deleteCache(cache);
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

