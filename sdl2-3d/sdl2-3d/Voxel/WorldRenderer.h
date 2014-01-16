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
	static const unsigned char AO_STRENGTH = 80;

	typedef std::unordered_map<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>, IVec3Hash> RenderChunkMap;

	/** Renders a VoxelWorld, will manage all the rendering related objects */
	WorldRenderer();
	~WorldRenderer();
	WorldRenderer(const WorldRenderer& copy) = delete;

	void render(VoxelWorld& world, const Camera& camera);

private:
	/** Calculate AO contribution for a vertex given the touching perpendicular faces*/
	inline static unsigned char getAO(bool side, bool side2, bool corner);
	/** Get a render chunk object containing the render related data for a chunk given a chunk position */
	void removeRenderChunk(const glm::ivec3& pos);
	const std::shared_ptr<VoxelRenderer::Chunk> getRenderChunk(const glm::ivec3& pos, const glm::vec3* const bounds);
	RenderChunkMap m_renderChunks;
	/** Renders all the block faces */
	VoxelRenderer m_renderer;
};

#endif //WORLD_RENDERER_H_
