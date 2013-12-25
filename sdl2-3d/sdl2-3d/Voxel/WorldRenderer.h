#ifndef WORLD_RENDERER_H_
#define WORLD_RENDERER_H_

#include "VoxelRenderer.h"
#include <unordered_map>

#include "../Engine/Utils/Comparables.h"

class VoxelWorld;
class Camera;
class TextureArray;



class WorldRenderer
{
public:
	typedef std::unordered_map<glm::ivec3, VoxelRenderer::Chunk*, IVec3Hash> RenderChunkMap;

	WorldRenderer();
	~WorldRenderer();
	WorldRenderer(const WorldRenderer& copy) = delete;

	void render(const VoxelWorld& world, const Camera& camera);

private:

	void renderDefault(const VoxelWorld& world, const Camera& camera, BlockID blockID, int x, int y, int z);

	VoxelRenderer::Chunk* getRenderChunk(const glm::ivec3& pos);
	RenderChunkMap m_renderChunks;
	VoxelRenderer m_renderer;
};

#endif //WORLD_RENDERER_H_
