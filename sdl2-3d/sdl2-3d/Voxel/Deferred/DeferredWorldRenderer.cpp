#include "DeferredWorldRenderer.h"
#include "PointLight.h"

#include <algorithm>
#include <glm\gtc\matrix_transform.hpp>

struct DistanceSort
{
	DistanceSort(const glm::vec3& pos) : camPos(pos) {};
	glm::vec3 camPos;

	bool operator()(const std::shared_ptr<VoxelRenderer::Chunk>& lhs, const std::shared_ptr<VoxelRenderer::Chunk>& rhs) {
		glm::vec3 delta1 = camPos - lhs->m_renderOffset;
		glm::vec3 delta2 = camPos - rhs->m_renderOffset;
		return (delta1.x + delta1.y + delta1.z) < (delta2.x + delta2.y + delta2.z);
	}
};

DeferredWorldRenderer::DeferredWorldRenderer()
	: m_gbuffer(1)
	//, m_sphere("Assets/Models/sphere.obj")
	, m_geometryShader("Assets/Shaders/gbuffer.vert", NULL, "Assets/Shaders/gbuffer.frag")
	, m_pointLightShader("Assets/Shaders/deferredpointlight.vert", NULL, "Assets/Shaders/deferredpointlight.frag")
	, m_quadShader("Assets/Shaders/quad.vert", NULL, "Assets/Shaders/quad.frag")
	, m_numLoadedChunks(0)
{
	m_quadShader.begin();
	m_quadShader.setUniform1i("u_depthTex", 0);
	m_quadShader.setUniform1i("u_colorTex", 1);
	m_quadShader.setUniform2f("u_screenSize", glm::vec2(Game::graphics.getScreenWidth(), Game::graphics.getScreenHeight()));
	m_quadShader.end();

	m_pointLightShader.begin();
	m_pointLightShader.setUniform1i("u_depthTex", 0);
	m_pointLightShader.setUniform1i("u_colorTex", 1);
	m_pointLightShader.setUniform2f("u_screenSize", glm::vec2(Game::graphics.getScreenWidth(), Game::graphics.getScreenHeight()));
	m_pointLightShader.end();
}

DeferredWorldRenderer::~DeferredWorldRenderer()
{

}

static const unsigned int MAX_MS_CHUNK_GEN_PER_FRAME = 2;

void DeferredWorldRenderer::findVisibleChunks(VoxelWorld& world, const Camera& camera)
{
	static const float sphereCullRad = CHUNK_SIZE / 2.0f;
	const float cullRangeSquared = (camera.m_far + CHUNK_SIZE) * (camera.m_far + CHUNK_SIZE);

	const PropertyManager& propertyManager = world.getPropertyManager();

	unsigned int startTicks = Game::getSDLTicks();

	glm::mat4 frustumCullMat = glm::transpose(camera.m_combinedMatrix);
	const glm::vec3 extent = glm::vec3(CHUNK_SIZE / 2);

	const ChunkManager::ChunkMap& chunks = world.getChunks();
	for (auto& it : chunks)
	{
		const std::unique_ptr<VoxelChunk>& chunk = it.second;
		const glm::ivec3& chunkPos = it.first;
		glm::vec3 chunkWorldPos = glm::vec3(chunkPos) * (float) CHUNK_SIZE;

		glm::vec3 dist = camera.m_position - chunkWorldPos;
		if (glm::dot(dist, dist) > cullRangeSquared)	// removing chunk data from gpu if out of camera.far range
		{
			removeRenderChunk(chunkPos);
			chunk->m_updated = false;
			continue;
		}

		//if (!Frustum::aabbInFrustum(chunkWorldPos, glm::vec3(8), frustumCullMat))
		//	continue;

		if (Game::getSDLTicks() - startTicks > MAX_MS_CHUNK_GEN_PER_FRAME)	//smooth out over multiple frames
			continue;	//continue, not break because we still want to remove chunks that are out of range even when past max time
		if (chunk->m_updated)
			continue;
		chunk->m_updated = true;

		buildChunk(chunk, world);
	}

	m_visibleChunkList.clear();
	m_visibleChunkList.reserve(m_numLoadedChunks);

	for (const std::pair<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>>& it : m_renderChunks)
	{
		glm::vec3 chunkBlockPos = glm::vec3(it.first * (int) CHUNK_SIZE) + extent;
		if (Frustum::aabbInFrustum(chunkBlockPos, glm::vec3(8), frustumCullMat))
			m_visibleChunkList.push_back(it.second);
	}
	std::sort(m_visibleChunkList.begin(), m_visibleChunkList.end(), DistanceSort(camera.m_position));
}

void DeferredWorldRenderer::doLight(const Camera& camera)
{
	PointLight p(camera.m_position);

	m_pointLights.push_back(p);
}

void DeferredWorldRenderer::render(VoxelWorld& world, const Camera& camera)
{
	//gather visible chunks in list
	findVisibleChunks(world, camera);
	doGeometryPass(world, camera);
	/*
	m_gbuffer.bindForWriting();
	glDepthMask(GL_TRUE);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	doGeometryPass(world, camera);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	m_gbuffer.unbind();

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	m_gbuffer.bindDepthTexture(0);
	m_gbuffer.bindForReading(1);
	glClear(GL_COLOR_BUFFER_BIT);

	doPointLightPass(world, camera);
	//doDirectionalLightPass(world, camera);
	*/
}

void DeferredWorldRenderer::doGeometryPass(const VoxelWorld& world, const Camera& camera)
{
	m_geometryShader.begin();
	world.getTileSet()->bind();
	m_geometryShader.setUniformMatrix4f("u_mvp", camera.m_combinedMatrix);
	for (const std::shared_ptr<VoxelRenderer::Chunk> chunk : m_visibleChunkList)
	{
		m_geometryShader.setUniform3f("u_chunkOffset", chunk->m_renderOffset);
		m_renderer.renderChunk(chunk);
	}
	m_geometryShader.end();
}

float calcPointLightBSphere(const PointLight& light)
{
	float maxChannel = fmax(fmax(light.color.r, light.color.g), light.color.b);

	float ret = (-light.linAtten + glm::sqrt(light.linAtten * light.linAtten - 4 * light.expAtten * (light.expAtten - 256 * maxChannel * light.diffuseIntensity)))
		/
		2 * light.expAtten;
	return ret;
}

void DeferredWorldRenderer::doPointLightPass(const VoxelWorld& world, const Camera& camera)
{
	m_pointLightShader.begin();
	m_pointLightShader.setUniformMatrix4f("u_mvp", camera.m_combinedMatrix);

	//m_pointLightShader.setUniformMatrix4f("u_invMvp", glm::inverse(camera.m_combinedMatrix));
	//m_pointLightShader.setUniform3f("u_camPos", camera.m_position);

	for (const PointLight& light : m_pointLights)
	{

		m_pointLightShader.setUniform3f("u_pointLightColor", light.color);
		m_pointLightShader.setUniform1f("u_pointLightAmbient", light.ambientIntensity);
		m_pointLightShader.setUniform1f("u_pointLightDiffuse", light.diffuseIntensity);
		m_pointLightShader.setUniform3f("u_pointLightPosition", light.position);
		m_pointLightShader.setUniform1f("u_pointLightConstAtten", light.constAtten);
		m_pointLightShader.setUniform1f("u_pointLightLinAtten", light.linAtten);
		m_pointLightShader.setUniform1f("u_pointLightExpAtten", light.expAtten);
	
		float boundingSphereScale = calcPointLightBSphere(light);
		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1), light.position), glm::vec3(boundingSphereScale));

		m_pointLightShader.setUniformMatrix4f("u_worldTrans", transform);
		//m_sphere.render();
	}
	m_pointLightShader.end();
}
void DeferredWorldRenderer::doDirectionalLightPass(const VoxelWorld& world, const Camera& camera)
{

}

const std::shared_ptr<VoxelRenderer::Chunk> DeferredWorldRenderer::getRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		const std::shared_ptr<VoxelRenderer::Chunk>& chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE);
		//printf("Creating: %i %i %i \n", pos.x, pos.y, pos.z);

		m_renderChunks.insert(std::make_pair(pos, chunk));
		return chunk;
	}
}

void DeferredWorldRenderer::removeRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
	{
		//printf("Removing: %i %i %i \n", pos.x, pos.y, pos.z);
		m_renderChunks.erase(it);
	}
}

void DeferredWorldRenderer::buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world)
{
	const glm::ivec3& chunkPos = chunk->m_pos;
	auto& renderChunk = getRenderChunk(chunkPos);

	m_renderer.beginChunk(renderChunk);
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				unsigned int blockIdx = VoxelChunk::getBlockIndex(glm::ivec3(x, y, z));
				const VoxelBlock& block = chunk->getBlock(blockIdx);
				if (block.id == 0)
					continue;

				const BlockProperties& properties = world.getBlockProperties(block.id);

				int worldX = chunkPos.x * CHUNK_SIZE + x;
				int worldY = chunkPos.y * CHUNK_SIZE + y;
				int worldZ = chunkPos.z * CHUNK_SIZE + z;

				const VoxelBlock* above = &world.getBlock(glm::ivec3(worldX, worldY + 1, worldZ));
				const VoxelBlock* below = &world.getBlock(glm::ivec3(worldX, worldY - 1, worldZ));
				const VoxelBlock* front = &world.getBlock(glm::ivec3(worldX + 1, worldY, worldZ));
				const VoxelBlock* back = &world.getBlock(glm::ivec3(worldX - 1, worldY, worldZ));
				const VoxelBlock* right = &world.getBlock(glm::ivec3(worldX, worldY, worldZ + 1));
				const VoxelBlock* left = &world.getBlock(glm::ivec3(worldX, worldY, worldZ - 1));
				// store the 6 adjacent blocks so we can check if they're solid.
				const VoxelBlock* faceValues[6] = { above, below, left, right, front, back };

				// for every face of the block
				for (int face = 0; face < 6; ++face)
				{
					// if the face is invisible, continue (if no airblock or transparant block touching face)
					if (faceValues[face]->solid)
						continue;
					Color8888 col = faceValues[face]->color;

					renderChunk->addFace((Face) face, x, y, z, properties.getTextureID((Face) face),
						col, col, col, col, false);
				}
			}
		}
	}
	m_renderer.endChunk(renderChunk);
}