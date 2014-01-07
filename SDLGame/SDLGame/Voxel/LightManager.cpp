#include "LightManager.h"

//TODO: everything

static const unsigned int LIGHT_APPLY_RADIUS_CHUNKS = 10;
static const unsigned int SKY_EMIT_HEIGHT = 255;

LightManager::LightManager()
{

}

LightManager::~LightManager()
{

}

void LightManager::applySkyLight(VoxelWorld& world, const Camera& camera)
{
	int chunkY = SKY_EMIT_HEIGHT % (int) CHUNK_SIZE;

	int rad = (int) LIGHT_APPLY_RADIUS_CHUNKS;

	//TODO: center around cam.
	for (int x = -rad; x < rad; ++x)
	{
		for (int z = -rad; z < rad; ++z)
		{
			std::shared_ptr<VoxelChunk> chunk = world.getChunk(glm::ivec3(x, chunkY, z));

		}
	}
}