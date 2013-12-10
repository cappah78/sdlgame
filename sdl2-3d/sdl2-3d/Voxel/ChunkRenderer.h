#ifndef CHUNK_RENDERER_H_
#define CHUNK_RENDERER_H_

#include "VoxelChunk.h"
#include "VoxelCache.h"
#include "PropertyManager.h"

class TextureArray;
class Camera;

/** Abstraction for 6 voxel caches to simplify rendering */
class ChunkRenderer
{
public:
	ChunkRenderer();
	ChunkRenderer(const ChunkRenderer& copy) = delete;
	~ChunkRenderer();

	void begin(const TextureArray* tileSet, float xOffset, float yOffset, float zOffset, const Camera* camera);
	void renderFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, int color1, int color2, int color3, int color4);
	void end();

private:
	const Camera* m_currCamera;
	VoxelCache m_voxelCache;
	const TextureArray* m_tileSet;
	VoxelCache::Cache* m_caches[6];
};

#endif //CHUNK_RENDERER_H_
