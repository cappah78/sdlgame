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
static const float CAMERA_FAR = 500.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 0, -10);
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
	, m_skyBox(CAMERA_FAR)
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
//#define BLOCK_TEST
#ifdef BLOCK_TEST
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

	//Color8888 color(26, 79, 57, 255);


	for (int i = 0; i < NUM_CHUNKS_X; ++i) 
	{
		for (int j = 0; j < NUM_CHUNKS_Y; ++j)
		{
			for (int k = 0; k < NUM_CHUNKS_Z; ++k) 
			{
				std::shared_ptr<VoxelRenderer::Chunk> chunk = m_voxelRenderer.createChunk(i * (float) CHUNK_SIZE, j * (float) CHUNK_SIZE, k * (float) CHUNK_SIZE);

				m_voxelRenderer.beginChunk(chunk);

				for (int x = 0; x < CHUNK_SIZE; x += 2) 
				{
					for (int y = 0; y < CHUNK_SIZE; y += 2)
					{
						for (int z = 0; z < CHUNK_SIZE; z += 2)
						{
							for (int face = 0; face < 6; ++face)	//iterate over the 6 different faces
							{
								// random-ish color
								int tot = i * 2 + j * 3+ k * 4 + x * 5 + y * 6 + z * 7;
								Color8888 col = Color8888(tot * 44, tot * 55, tot * 66, tot * 10);

								int textureId = (x + y + z + i + j + k) % images.size();
								chunk->addFace((VoxelRenderer::Face) face, x, y, z, textureId, col, col, col, col);
							}
						}
					}
				}

				m_voxelRenderer.endChunk(chunk);
				m_chunkRenderData.push_back(chunk);
			}
		}
	}
#endif //BLOCK_TEST
}

void GameScreen::render(float deltaSec)
{
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cameraController.update(deltaSec);
	m_camera.update();

#ifdef BLOCK_TEST
	m_voxelRenderer.beginRender(m_tileSet);
	for (std::shared_ptr<VoxelRenderer::Chunk> chunk : m_chunkRenderData)
		m_voxelRenderer.renderChunk(chunk, m_camera);
	m_voxelRenderer.endRender();
#else
	m_worldRenderer.render(m_world, m_camera);
#endif //BLOCK_TEST

	m_skyBox.render(m_camera);

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
	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

