#include "WorldRenderer.h"

#include "VoxelWorld.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "PropertyManager.h"
#include <iostream>
#include <stdio.h>


WorldRenderer::WorldRenderer()
{
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
	
}

WorldRenderer::~WorldRenderer()
{
	delete m_tileSet;
}

void WorldRenderer::render(const VoxelWorld& world, const Camera& camera)
{

	const ChunkManager::ChunkMap& chunks = world.getChunks();
	const glm::vec3& camPos = camera.m_position;
	int chunkX = (int) glm::floor(camPos.x / (float) CHUNK_SIZE);
	int chunkY = (int) glm::floor(camPos.y / (float) CHUNK_SIZE);
	int chunkZ = (int) glm::floor(camPos.z / (float) CHUNK_SIZE);
	//glm::ivec3 chunkPos(chunkX, chunkY, chunkZ);
	
	for (auto it = chunks.begin(); it != chunks.end(); ++it)
	{
		VoxelChunk* chunk = it->second;
		glm::ivec3 chunkPos = it->first;

		if (chunk->m_updated)
			continue;
		chunk->m_updated = true;
		VoxelRenderer::Chunk* renderChunk = getRenderChunk(chunkPos);
		m_renderer.beginChunk(renderChunk);
		printf("pos: %f, %f, %f : %i, %i, %i \n", camPos.x, camPos.y, camPos.z, chunkPos.x, chunkPos.y, chunkPos.z);


		BlockID* blocks = chunk->getBlocks();
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					unsigned int blockIdx = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z;
					BlockID id = blocks[blockIdx];
					if (id == 0)
						continue;

					BlockRenderType type = world.getPropertyManager().getRenderType(id);
					Color8888 color(0, 0, 0, 255);

					switch (type)
					{
					case DEFAULT:
					{
							DefaultBlockRenderProperties properties = world.getPropertyManager().getDefaultRenderProperties(id);
							//TODO: face culling
							renderChunk->addFace(VoxelRenderer::TOP, x, y, z, (TextureID) properties.topTexture, color, color, color, color);
							renderChunk->addFace(VoxelRenderer::BOTTOM, x, y, z, (TextureID)  properties.bottomTexture, color, color, color, color);
							renderChunk->addFace(VoxelRenderer::LEFT, x, y, z, (TextureID)  properties.leftTexture, color, color, color, color);
							renderChunk->addFace(VoxelRenderer::RIGHT, x, y, z, (TextureID) properties.rightTexture, color, color, color, color);
							renderChunk->addFace(VoxelRenderer::FRONT, x, y, z, (TextureID) properties.frontTexture, color, color, color, color);
							renderChunk->addFace(VoxelRenderer::BACK, x, y, z, (TextureID) properties.backTexture, color, color, color, color);
					}

						break;
					case MESH:
						break;
					}
				}
			}
		}
		m_renderer.endChunk(renderChunk);
	}

	m_renderer.beginRender(world.getTileSet());
	for (auto it = m_renderChunks.begin(); it != m_renderChunks.end(); ++it)
	{
		m_renderer.renderChunk(it->second, camera);
	}
	m_renderer.endRender();
}

void WorldRenderer::renderDefault(const VoxelWorld& world, const Camera& camera, BlockID blockID, int x, int y, int z)
{

}

VoxelRenderer::Chunk* WorldRenderer::getRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		printf("createchunk: %i, %i, %i \n", pos.x, pos.y, pos.z);
		VoxelRenderer::Chunk* chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE);
		m_renderChunks.insert(std::make_pair(pos, chunk));
		return chunk;
	}
}