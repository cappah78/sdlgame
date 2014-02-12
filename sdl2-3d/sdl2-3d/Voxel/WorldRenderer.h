#pragma once

#include "VoxelRenderer.h"
#include "../Engine/Utils/Comparables.h"

#include "../Engine/Graphics/Model/IGraphicsProvider.h"

#include <unordered_map>

class VoxelWorld;
class Camera;
class TextureArray;

class WorldRenderer
{
public:
	typedef std::unordered_map<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>, IVec3Hash, IVec3Equality> RenderChunkMap;

	static const unsigned char AO_STRENGTH = 80;

	WorldRenderer();
	~WorldRenderer();
	WorldRenderer(const WorldRenderer& copy) = delete;

	void render(VoxelWorld& world, const Camera& camera);

private:

	inline static unsigned char getAO(bool side, bool side2, bool corner);

	void removeRenderChunk(const glm::ivec3& pos);
	void buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world);
	const std::shared_ptr<VoxelRenderer::Chunk> getRenderChunk(const glm::ivec3& pos);

	struct PerFrameUniformData
	{
		glm::mat4 mvp;
		glm::vec3 camPos;
		float fogStart;
		glm::vec3 fogColor;
		float fogEnd;
	} m_perFrameUniformData;

	struct PerInstanceUniformData
	{
		glm::vec3 chunkOffset;
		float padding;
	} m_perInstanceUniformData;

	RenderChunkMap m_renderChunks;
	unsigned int m_numLoadedChunks;

	VoxelRenderer m_renderer;
	std::vector<std::shared_ptr<VoxelRenderer::Chunk>> m_visibleChunkList;

	std::auto_ptr<IConstantBuffer> m_perFrameUniformDataBuffer;
	std::auto_ptr<IConstantBuffer> m_perInstanceUniformDataBuffer;
	std::auto_ptr<IShader> m_iVoxelShader;
};