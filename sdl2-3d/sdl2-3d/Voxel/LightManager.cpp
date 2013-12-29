#include "LightManager.h"

//TODO: everything

LightManager::LightManager()
{

}

LightManager::~LightManager()
{

}

void LightManager::applySkyLight(VoxelWorld& world, const Camera& camera)
{
	const ChunkManager::ChunkMap& chunks = world.getChunks();
}