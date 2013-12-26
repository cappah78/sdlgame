#include "WorldRenderer.h"

#include "VoxelWorld.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "PropertyManager.h"

WorldRenderer::WorldRenderer()
{
	
}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::render(const VoxelWorld& world, const Camera& camera)
{
	const ChunkManager::ChunkMap& chunks = world.getChunks();
	const glm::vec3& camPos = camera.m_position;
	int chunkX = (int) glm::floor(camPos.x / (float) CHUNK_SIZE);
	int chunkY = (int) glm::floor(camPos.y / (float) CHUNK_SIZE);
	int chunkZ = (int) glm::floor(camPos.z / (float) CHUNK_SIZE);
	
	for (auto it = chunks.begin(); it != chunks.end(); ++it)
	{
		VoxelChunk* chunk = it->second;
		const glm::ivec3& chunkPos = chunk->m_pos;

		if (chunk->m_updated)
			continue;
		chunk->m_updated = true;
		const std::shared_ptr<VoxelRenderer::Chunk>& renderChunk = getRenderChunk(chunkPos);
		m_renderer.beginChunk(renderChunk);

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

					const PropertyManager& propertyManager = world.getPropertyManager();
					Color8888 color(0, 0, 0, 255);

					BlockRenderData properties = propertyManager.getBlockRenderData(id);
					int worldX = chunkPos.x * CHUNK_SIZE + x;
					int worldY = chunkPos.y * CHUNK_SIZE + y;
					int worldZ = chunkPos.z * CHUNK_SIZE + z;

					BlockID above = world.getBlockID(glm::ivec3(worldX, worldY + 1, worldZ));
					BlockID below = world.getBlockID(glm::ivec3(worldX, worldY - 1, worldZ));
					BlockID front = world.getBlockID(glm::ivec3(worldX + 1, worldY, worldZ));
					BlockID back = world.getBlockID(glm::ivec3(worldX - 1, worldY, worldZ));
					BlockID left = world.getBlockID(glm::ivec3(worldX, worldY, worldZ - 1));
					BlockID right = world.getBlockID(glm::ivec3(worldX, worldY, worldZ + 1));

					if (above == 0 || propertyManager.getBlockRenderData(above).isTransparent)
						renderChunk->addFace(VoxelRenderer::TOP, x, y, z, properties.topTexture, color, color, color, color);
					if (below == 0 || propertyManager.getBlockRenderData(below).isTransparent)
						renderChunk->addFace(VoxelRenderer::BOTTOM, x, y, z, properties.bottomTexture, color, color, color, color);
					if (left == 0 || propertyManager.getBlockRenderData(left).isTransparent)
						renderChunk->addFace(VoxelRenderer::LEFT, x, y, z, properties.leftTexture, color, color, color, color);
					if (right == 0 || propertyManager.getBlockRenderData(right).isTransparent)
						renderChunk->addFace(VoxelRenderer::RIGHT, x, y, z, properties.rightTexture, color, color, color, color);
					if (front == 0 || propertyManager.getBlockRenderData(front).isTransparent)
						renderChunk->addFace(VoxelRenderer::FRONT, x, y, z, properties.frontTexture, color, color, color, color);
					if (back == 0 || propertyManager.getBlockRenderData(back).isTransparent)
						renderChunk->addFace(VoxelRenderer::BACK, x, y, z, properties.backTexture, color, color, color, color);
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

const std::shared_ptr<VoxelRenderer::Chunk> WorldRenderer::getRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		//printf("createchunk: %i, %i, %i \n", pos.x, pos.y, pos.z);
		const std::shared_ptr<VoxelRenderer::Chunk>& chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE);
		m_renderChunks.insert(std::make_pair(pos, chunk));
		return chunk;
	}
}