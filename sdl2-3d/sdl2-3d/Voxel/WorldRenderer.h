#pragma once

#include "VoxelRenderer.h"
#include "../Engine/Utils/Comparables.h"

#include <unordered_map>

class IConstantBuffer;
class IShader;

class VoxelWorld;
class Camera;

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
		glm::vec4 camPos;
		glm::vec4 fogColor;
		float fogStart;
		float fogEnd;
		float padding;
		float padding2;
	} m_perFrameUniformData;

	struct PerChunkUniformData
	{
		glm::vec4 chunkOffset;
	} m_perChunkUniformData;

	RenderChunkMap m_renderChunks;
	unsigned int m_numLoadedChunks;

	VoxelRenderer m_renderer;
	std::vector<std::shared_ptr<VoxelRenderer::Chunk>> m_visibleChunkList;

	std::unique_ptr<IConstantBuffer> m_perFrameUniformDataBuffer;
	std::unique_ptr<IConstantBuffer> m_perChunkUniformDataBuffer;
	std::unique_ptr<IShader> m_iVoxelShader;
};