#ifndef CHUNK_RENDERER_H_
#define CHUNK_RENDERER_H_

#include "VoxelChunk.h"
#include "VoxelCache.h"
#include "PropertyManager.h"

/** Abstraction for 6 voxel caches to simplify rendering cubes */
class ChunkRenderer
{
public:
	ChunkRenderer(float offsetX, float offsetY, float offsetZ);
	ChunkRenderer(const ChunkRenderer& copy) = delete;
	~ChunkRenderer();

	void begin();
	void renderFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, float color1, float color2, float color3, float color4);
	void end();

private:
	float m_offsetX, m_offsetY, m_offsetZ;

	VoxelCache m_voxelCache;

	VoxelCache::Cache* m_topFaceCache;
	VoxelCache::Cache* m_bottomFaceCache;
	VoxelCache::Cache* m_leftFaceCache;
	VoxelCache::Cache* m_rightFaceCache;
	VoxelCache::Cache* m_frontFaceCache;
	VoxelCache::Cache* m_backFaceCache;
};

#endif //CHUNK_RENDERER_H_
