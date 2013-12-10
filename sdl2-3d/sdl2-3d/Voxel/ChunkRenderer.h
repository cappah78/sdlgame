#ifndef CHUNK_RENDERER_H_
#define CHUNK_RENDERER_H_

#include "VoxelChunk.h"
#include "VoxelCache.h"
#include "PropertyManager.h"
#include "../Engine/Graphics/Color8888.h"

class TextureArray;
class Camera;

/** Abstraction for 6 voxel caches to simplify rendering */
class ChunkRenderData
{
	friend class ChunkRenderer;
public:
	float m_xOffset, m_yOffset, m_zOffset;
	void addFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4);
private:
	ChunkRenderData(float xOffset, float yOffset, float zOffset)
		: m_xOffset(xOffset), m_yOffset(yOffset), m_zOffset(zOffset), m_begun(false)
	{};
	~ChunkRenderData() {};
	ChunkRenderData(const ChunkRenderData& copy) = delete;

	bool m_begun;
	VoxelCache::Cache* m_caches[6];
};

class ChunkRenderer
{
public:
	ChunkRenderer();
	ChunkRenderer(const ChunkRenderer& copy) = delete;
	~ChunkRenderer();

	ChunkRenderData* const createChunkRenderData(float xOffset, float yOffset, float zOffset);
	void deleteChunkRenderData(ChunkRenderData* const chunkRenderData);
	void beginAdd(ChunkRenderData* const chunkRenderData);
	void endAdd(ChunkRenderData* const chunkRenderData);

	void beginRender(const TextureArray* tileSet);
	void renderChunk(const ChunkRenderData* const renderData, const Camera& camera);
	void endRender();

private:
	VoxelCache m_voxelCache;
};

#endif //CHUNK_RENDERER_H_
