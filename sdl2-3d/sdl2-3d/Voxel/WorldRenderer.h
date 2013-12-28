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
	typedef std::unordered_map<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>, IVec3Hash> RenderChunkMap;

	WorldRenderer();
	~WorldRenderer();
	WorldRenderer(const WorldRenderer& copy) = delete;

	void render(const VoxelWorld& world, const Camera& camera);

private:

	inline static unsigned char getAO(bool side, bool side2, bool corner);
	const std::shared_ptr<VoxelRenderer::Chunk> getRenderChunk(const glm::ivec3& pos);
	RenderChunkMap m_renderChunks;
	VoxelRenderer m_renderer;
};

#endif //WORLD_RENDERER_H_
