#pragma once

#include "../VoxelWorld.h"
#include "../VoxelRenderer.h"
#include "../../Engine/Graphics/Camera.h"
#include "../../Engine/Graphics/GL/GBuffer.h"
#include "../../Engine/Graphics/GL/Shader.h"
#include "../../Engine/Graphics/GL/Mesh.h"
#include "PointLight.h"

#include "../../Engine/Graphics/Renderers/QuadRenderer.h"

class DeferredWorldRenderer
{
public:
	typedef std::unordered_map<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>, IVec3Hash, IVec3Equality> RenderChunkMap;

	DeferredWorldRenderer();
	~DeferredWorldRenderer();
	DeferredWorldRenderer(const DeferredWorldRenderer& copy) = delete;

	void render(VoxelWorld& world, const Camera& camera);
	void doLight(const Camera& camera);
private:

	void findVisibleChunks(VoxelWorld& world, const Camera& camera);

	void doGeometryPass(const VoxelWorld& world, const Camera& camera);
	void beginLightPasses(const VoxelWorld& world, const Camera& camera);
	void doPointLightPass(const VoxelWorld& world, const Camera& camera);
	void doDirectionalLightPass(const VoxelWorld& world, const Camera& camera);

	void buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world);
	void buildChunkWithAO(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world);

	void removeRenderChunk(const glm::ivec3& pos);
	const std::shared_ptr<VoxelRenderer::Chunk> getRenderChunk(const glm::ivec3& pos);

	RenderChunkMap m_renderChunks;
	unsigned int m_numLoadedChunks;
	VoxelRenderer m_renderer;

	//Mesh m_sphere;

	GBuffer m_gbuffer;
	Shader m_geometryShader;

	Shader m_pointLightShader;
	Shader m_directionalLightShader;

	Shader m_quadShader;
	QuadRenderer m_quadRenderer;

	std::vector<std::shared_ptr<VoxelRenderer::Chunk>> m_visibleChunkList;
	std::vector<PointLight> m_pointLights;
};