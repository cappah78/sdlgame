#pragma once

#include "VoxelRenderer.h"
#include "../Engine/Utils/Comparables.h"

#include "../Engine/Graphics/Renderers/QuadRenderer.h"
#include "../Engine/Graphics/GL/Texture.h"
#include <unordered_map>

class VoxelWorld;
class Camera;
class TextureArray;

class WorldRenderer
{
public:
	static const unsigned char AO_STRENGTH = 80;

	typedef std::unordered_map<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>, IVec3Hash, IVec3Equality> RenderChunkMap;

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
	const std::shared_ptr<VoxelRenderer::Chunk> getRenderChunk(const glm::ivec3& pos);

	void buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world);

	RenderChunkMap m_renderChunks;

	Shader m_voxelShader;
	VoxelRenderer m_renderer;

	GBuffer m_gbuffer;
	Shader m_lightShader;

	unsigned int m_numLoadedChunks;

	std::vector<float> m_getPixelBuffer;

	std::vector<std::shared_ptr<VoxelRenderer::Chunk>> m_visibleChunkList;
};