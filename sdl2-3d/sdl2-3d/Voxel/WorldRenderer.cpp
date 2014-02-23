#include "WorldRenderer.h"

#include "VoxelWorld.h"
#include "PropertyManager.h"

#include "../Engine/Graphics/Camera.h"
#include "../Game.h"

#include "../Engine/Graphics/Model/IShader.h"
#include "../Engine/Graphics/Model/IConstantBuffer.h"
#include "../Engine/Graphics/Model/ITextureArray.h"

#include <algorithm>
#include <glm\gtc\matrix_transform.hpp>

static unsigned int getSolidBlockMask(int xOffset, int yOffset, int zOffset)
{
	return 1 << ((zOffset + 1) + 3 * ((yOffset + 1) + 3 * (xOffset + 1)));
}

inline unsigned char getAO(bool side, bool side2, bool corner)
{
	return (side && side2 ? 3 * WorldRenderer::AO_STRENGTH : (side + corner + side2) * WorldRenderer::AO_STRENGTH);
}

static const unsigned int SOLID_FACE_MASKS[6] =
{
	getSolidBlockMask(0, 1, 0),
	getSolidBlockMask(0, -1, 0),
	getSolidBlockMask(0, 0, -1),
	getSolidBlockMask(0, 0, 1),
	getSolidBlockMask(1, 0, 0),
	getSolidBlockMask(-1, 0, 0)
};

//face, vertex, offsetvec	//TODO: refactor so offsets are calculated programmatically instead of from this array.
static const unsigned int AO_CHECKS_MASKS[6][4][3] =
{
	{ //above
		{ //v1
			getSolidBlockMask(0, 1, -1),	//left
			getSolidBlockMask(1, 1, 0),	//front
			getSolidBlockMask(1, 1, -1)	//frontleft
		},
		{ //v2
			getSolidBlockMask(1, 1, 0),	//front
			getSolidBlockMask(0, 1, 1),	//right
			getSolidBlockMask(1, 1, 1)		//frontright
		},
		{ //v3
			getSolidBlockMask(0, 1, 1),	//right
			getSolidBlockMask(-1, 1, 0),	//back
			getSolidBlockMask(-1, 1, 1)	//backright
		},
		{ //v4
			getSolidBlockMask(0, 1, -1),	//left
			getSolidBlockMask(-1, 1, 0),	//back
			getSolidBlockMask(-1, 1, -1)	//backleft
		}
	},
	{ //below
		{ //v1
			getSolidBlockMask(0, -1, 1),	//right
			getSolidBlockMask(1, -1, 0),	//front
			getSolidBlockMask(1, -1, 1)	//frontright
		},
		{ //v2
			getSolidBlockMask(1, -1, 0),	//front
			getSolidBlockMask(0, -1, -1),	//left
			getSolidBlockMask(1, -1, -1)	//frontleft
		},
		{ //v3
			getSolidBlockMask(0, -1, -1),	//left
			getSolidBlockMask(-1, -1, 0),	//back
			getSolidBlockMask(-1, -1, -1)	//backleft
		},
		{ //v4
			getSolidBlockMask(0, -1, 1),	//right
			getSolidBlockMask(-1, -1, 0),	//back
			getSolidBlockMask(-1, -1, 1)	//backright
		}
	},
	{ //left
		{ //v1
			getSolidBlockMask(0, 1, -1),
			getSolidBlockMask(-1, 0, -1),
			getSolidBlockMask(-1, 1, -1)
		},
		{ //v2
			getSolidBlockMask(-1, 0, -1),
			getSolidBlockMask(0, -1, -1),
			getSolidBlockMask(-1, -1, -1)
		},
		{ //v3
			getSolidBlockMask(0, -1, -1),
			getSolidBlockMask(1, 0, -1),
			getSolidBlockMask(1, -1, -1)
		},
		{ //v4
			getSolidBlockMask(0, 1, -1),
			getSolidBlockMask(1, 0, -1),
			getSolidBlockMask(1, 1, -1)
		}
	},
	{ //right
		{ //v1
			getSolidBlockMask(0, 1, 1),
			getSolidBlockMask(1, 0, 1),
			getSolidBlockMask(1, 1, 1)
		},
		{ //v2
			getSolidBlockMask(1, 0, 1),
			getSolidBlockMask(0, -1, 1),
			getSolidBlockMask(1, -1, 1)
		},
		{ //v3
			getSolidBlockMask(0, -1, 1),
			getSolidBlockMask(-1, 0, 1),
			getSolidBlockMask(-1, -1, 1)
		},
		{ //v4
			getSolidBlockMask(0, 1, 1),
			getSolidBlockMask(-1, 0, 1),
			getSolidBlockMask( -1, 1, 1)
		}
	},
	{ //front
		{ //v2
			getSolidBlockMask(1, 1, 0),
			getSolidBlockMask(1, 0, -1),
			getSolidBlockMask(1, 1, -1)
		},
		{ //v3
			getSolidBlockMask(1, 0, -1),
			getSolidBlockMask(1, -1, 0),
			getSolidBlockMask(1, -1, -1)
		},
		{ //v4
			getSolidBlockMask(1, 0, 1),
			getSolidBlockMask(1, -1, 0),
			getSolidBlockMask(1, -1, 1)
		},
		{ //v1
			getSolidBlockMask(1, 0, 1),
			getSolidBlockMask(1, 1, 0),
			getSolidBlockMask(1, 1, 1)
		}
	},
	{ //back
		{ //v2
			getSolidBlockMask(-1, 1, 0),
			getSolidBlockMask(-1, 0, 1),
			getSolidBlockMask(-1, 1, 1)
		},
		{ //v3
			getSolidBlockMask(-1, 0, 1),
			getSolidBlockMask(-1, -1, 0),
			getSolidBlockMask(-1, -1, 1)
		},
		{ //v4
			getSolidBlockMask(-1, 0, -1),
			getSolidBlockMask(-1, -1, 0),
			getSolidBlockMask(-1, -1, -1)
		},
		{ //v1
			getSolidBlockMask(-1, 0, -1),
			getSolidBlockMask(-1, 1, 0),
			getSolidBlockMask(-1, 1, -1)
		}
	}
};

WorldRenderer::WorldRenderer()
	: m_numLoadedChunks(0)
{
	m_iVoxelShader = Game::graphics.getGraphicsProvider().createShaderFromFile("Assets/Shaders/voxelshader.vert", "Assets/Shaders/voxelshader.frag");
	m_perFrameUniformDataBuffer = Game::graphics.getGraphicsProvider().createConstantBuffer(m_iVoxelShader, 0, "PerFrameData", IConstantBufferParameters());
	m_perChunkUniformDataBuffer = Game::graphics.getGraphicsProvider().createConstantBuffer(m_iVoxelShader, 1, "PerChunkData", IConstantBufferParameters());

	m_perFrameUniformData.fogColor = glm::vec4(0.4f, 0.7f, 1.0f, 1.0f);
}

WorldRenderer::~WorldRenderer()
{
	m_visibleChunkList.clear();
}

void WorldRenderer::render(VoxelWorld& world, const Camera& camera)
{
	updateChunks(world, camera);
	findVisibleChunks(world, camera);
	renderVisibleChunks(world, camera);
}

static const unsigned int MAX_MS_CHUNK_GEN_PER_FRAME = 2;

void WorldRenderer::updateChunks(VoxelWorld& world, const Camera& camera)
{
	const float cullRangeSquared = (camera.m_far + CHUNK_SIZE) * (camera.m_far + CHUNK_SIZE);
	unsigned int startTicks = Game::getSDLTicks();

	for (const auto& it : world.getChunks())
	{
		const std::unique_ptr<VoxelChunk>& chunk = it.second;
		const glm::ivec3& chunkPos = it.first;

		glm::vec3 chunkWorldPos = glm::vec3(chunkPos) * (float) CHUNK_SIZE;
		glm::vec3 dist = camera.m_position - chunkWorldPos;

		if (glm::dot(dist, dist) > cullRangeSquared)	// removing chunk data from gpu if out of camera.far range
		{
			unloadRenderChunk(chunkPos);
			chunk->m_shouldUpdate = true;
			continue;
		}

		if (Game::getSDLTicks() - startTicks > MAX_MS_CHUNK_GEN_PER_FRAME)	//smooth out over multiple frames
			continue;	//continue, not break because we still want to remove chunks that are out of range even when past max time
		if (!chunk->m_shouldUpdate)
			continue;
		chunk->m_shouldUpdate = false;

		buildChunk(chunk, world);
	}
}

void WorldRenderer::findVisibleChunks(VoxelWorld& world, const Camera& camera)
{
	glm::mat4 frustumCullMat = glm::transpose(camera.m_combinedMatrix);
	const glm::vec3 extent = glm::vec3(CHUNK_SIZE / 2);

	m_visibleChunkList.clear();
	m_visibleChunkList.reserve(m_numLoadedChunks);

	for (const std::pair<glm::ivec3, std::shared_ptr<VoxelRenderer::Chunk>>& it : m_renderChunks)
	{
		glm::vec3 chunkBlockPos = glm::vec3(it.first * (int) CHUNK_SIZE) + extent;
		if (Frustum::aabbInFrustum(chunkBlockPos, glm::vec3(8), frustumCullMat))
			m_visibleChunkList.push_back(it.second);
	}
}

void WorldRenderer::renderVisibleChunks(VoxelWorld& world, const Camera& camera)
{
	world.getTileSet()->bind();

	m_iVoxelShader->begin();

	m_perFrameUniformData.camPos = glm::vec4(camera.m_position, 0.0f);
	m_perFrameUniformData.fogEnd = camera.m_far;
	m_perFrameUniformData.fogStart = camera.m_far * 0.7f;
	m_perFrameUniformData.mvp = camera.m_combinedMatrix;

	m_perFrameUniformDataBuffer->update(&m_perFrameUniformData, sizeof(m_perFrameUniformData));

	for (const std::shared_ptr<VoxelRenderer::Chunk>& chunk : m_visibleChunkList)
	{
		m_perChunkUniformData.chunkOffset = glm::vec4(chunk->m_renderOffset, 0.0f);
		m_perChunkUniformDataBuffer->update(&m_perChunkUniformData, sizeof(m_perChunkUniformData));
		m_renderer.renderChunk(chunk);
	}
	m_iVoxelShader->end();
}

void WorldRenderer::buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world)
{
	const glm::ivec3& chunkPos = chunk->m_pos;
	const std::shared_ptr<VoxelRenderer::Chunk>& renderChunk = loadRenderChunk(chunkPos);

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

				unsigned int solidBits = world.getSolidSurroundingBlockBits(glm::ivec3(worldX, worldY, worldZ));

				// for every face of the block
				for (int face = 0; face < 6; ++face)
				{
					// if the face is invisible, continue (if no airblock or transparant block touching face)
					if (solidBits & SOLID_FACE_MASKS[face])
						continue;

					unsigned char vertexAO[4];
					// for every vertex of the face
					for (int vertex = 0; vertex < 4; ++vertex)
					{
						bool isSolid[3];

						// sample 3 times per vertex, for the touching blocks; side, side, corner
						for (int sample = 0; sample < 3; ++sample)	
						{
							unsigned int bitMask = AO_CHECKS_MASKS[face][vertex][sample];
							isSolid[sample] = (solidBits & bitMask) != 0;
						}

						vertexAO[vertex] = getAO(isSolid[0], isSolid[1], isSolid[2]);
					}

					// Flip quad when needed to avoid asymmetric vertex color interpolation 
					bool flipQuad = vertexAO[1] + vertexAO[3] > vertexAO[0] + vertexAO[2];
					renderChunk->addFace((Face) face, x, y, z, properties.getTextureID((Face) face),
						vertexAO[0], vertexAO[3], vertexAO[1], vertexAO[2], flipQuad);
				}
			}
		}
	}
	m_renderer.endChunk(renderChunk);
}

const std::shared_ptr<VoxelRenderer::Chunk> WorldRenderer::loadRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		const std::shared_ptr<VoxelRenderer::Chunk>& chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE);

		m_renderChunks.insert(std::make_pair(pos, chunk));
		m_numLoadedChunks++;
		return chunk;
	}
}

void WorldRenderer::unloadRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
	{
		m_renderChunks.erase(it);
		m_numLoadedChunks--;
	}
}