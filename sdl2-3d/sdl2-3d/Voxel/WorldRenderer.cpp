#include "WorldRenderer.h"

#include "VoxelWorld.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "PropertyManager.h"
#include "../Game.h"
#include "../Engine/Utils/CheckGLError.h"
#include <algorithm>

//face, vertex, offsetvec	//TODO: refactor so offsets are calculated programmatically instead of from this array.
static const char AO_CHECKS_OFFSET[6][4][3][3] =
{
	{ //above
		{ //v1
			{ 0, 1, -1 },	//left
			{ 1, 1, 0 },	//front
			{ 1, 1, -1 }	//frontleft
		},
		{ //v2
			{ 1, 1, 0 },	//front
			{ 0, 1, 1 },	//right
			{ 1, 1, 1 }		//frontright
		},
		{ //v3
			{ 0, 1, 1 },	//right
			{ -1, 1, 0 },	//back
			{ -1, 1, 1 }	//backright
		},
		{ //v4
			{ 0, 1, -1 },	//left
			{ -1, 1, 0 },	//back
			{ -1, 1, -1 }	//backleft
		}
	},
	{ //below
		{ //v1
			{ 0, -1, 1 },	//right
			{ 1, -1, 0 },	//front
			{ 1, -1, 1 }	//frontright
		},
		{ //v2
			{ 1, -1, 0 },	//front
			{ 0, -1, -1 },	//left
			{ 1, -1, -1 }	//frontleft
		},
		{ //v3
			{ 0, -1, -1 },	//left
			{ -1, -1, 0 },	//back
			{ -1, -1, -1 }	//backleft
		},
		{ //v4
			{ 0, -1, 1 },	//right
			{ -1, -1, 0 },	//back
			{ -1, -1, 1 }	//backright
		}
	},
	{ //left
		{ //v1
			{ 0, 1, -1 },
			{ -1, 0, -1 },
			{ -1, 1, -1 }
		},
		{ //v2
			{ -1, 0, -1 },
			{ 0, -1, -1 },
			{ -1, -1, -1 }
		},
		{ //v3
			{ 0, -1, -1 },
			{ 1, 0, -1 },
			{ 1, -1, -1 }
		},
		{ //v4
			{ 0, 1, -1 },
			{ 1, 0, -1 },
			{ 1, 1, -1 }
		}
	},
	{ //right
		{ //v1
			{ 0, 1, 1 },
			{ 1, 0, 1 },
			{ 1, 1, 1 }
		},
		{ //v2
			{ 1, 0, 1 },
			{ 0, -1, 1 },
			{ 1, -1, 1 }
		},
		{ //v3
			{ 0, -1, 1 },
			{ -1, 0, 1 },
			{ -1, -1, 1 }
		},
		{ //v4
			{ 0, 1, 1 },
			{ -1, 0, 1 },
			{ -1, 1, 1 }
		}
	},
	{ //front
		{ //v2
			{ 1, 1, 0 },
			{ 1, 0, -1 },
			{ 1, 1, -1 }
		},
		{ //v3
			{ 1, 0, -1 },
			{ 1, -1, 0 },
			{ 1, -1, -1 }
		},
		{ //v4
			{ 1, 0, 1 },
			{ 1, -1, 0 },
			{ 1, -1, 1 }
		},
		{ //v1
			{ 1, 0, 1 },
			{ 1, 1, 0 },
			{ 1, 1, 1 }
		}

	},
	{ //back
		{ //v2
			{ -1, 1, 0 },
			{ -1, 0, 1 },
			{ -1, 1, 1 }
		},
		{ //v3
			{ -1, 0, 1 },
			{ -1, -1, 0 },
			{ -1, -1, 1 }
		},
		{ //v4
			{ -1, 0, -1 },
			{ -1, -1, 0 },
			{ -1, -1, -1 }
		},
		{ //v1
			{ -1, 0, -1 },
			{ -1, 1, 0 },
			{ -1, 1, -1 }
		}
	}
};


WorldRenderer::WorldRenderer()
	: m_voxelShader("Assets/Shaders/voxelshader.vert", NULL, "Assets/Shaders/voxelshader.frag")
	, m_numLoadedChunks(0)
{
	m_voxelShader.begin();
	m_voxelShader.setUniform3f("u_fogColor", glm::vec3(0.4f, 0.7f, 1.0f));        //same as clearcolor
	m_voxelShader.end();

	CHECK_GL_ERROR();
}

WorldRenderer::~WorldRenderer()
{
	m_visibleChunkList.clear();
}

inline unsigned char WorldRenderer::getAO(bool side, bool side2, bool corner)
{
	return side && side2 ? 2 * AO_STRENGTH : (side + corner + side2) * AO_STRENGTH;
	/* == 
	if (side && side2)
		return 2 * AO_STRENGTH;
	else
		return (side + side2 + corner) * AO_STRENGTH;
	*/
}

struct DistanceSort
{
	DistanceSort(const glm::vec3& pos) : camPos(pos) {};
	glm::vec3 camPos;

	bool operator()(const std::shared_ptr<VoxelRenderer::Chunk>& lhs, const std::shared_ptr<VoxelRenderer::Chunk>& rhs) {
		glm::vec3 delta1 = camPos - lhs->m_renderOffset;
		glm::vec3 delta2 = camPos - rhs->m_renderOffset;

		//return glm::dot(delta1, delta1) < glm::dot(delta2, delta2);

		//faster but less accurate
		return (delta1.x + delta1.y + delta1.z) < (delta2.x + delta2.y + delta2.z);
	}
};

static const unsigned int MAX_MS_CHUNK_GEN_PER_FRAME = 2;

void WorldRenderer::render(VoxelWorld& world, const Camera& camera)
{
	static const float sphereCullRad = CHUNK_SIZE / 2.0f;
	const float cullRangeSquared = camera.m_far * camera.m_far;

	const PropertyManager& propertyManager = world.getPropertyManager();

	unsigned int startTicks = Game::getSDLTicks();

	const ChunkManager::ChunkMap& chunks = world.getChunks();
	for (auto& it : chunks)
	{
		const std::unique_ptr<VoxelChunk>& chunk = it.second;
		const glm::ivec3& chunkPos = it.first;
		glm::vec3 chunkWorldPos = glm::vec3(chunkPos.x * (float) CHUNK_SIZE, chunkPos.y * (float) CHUNK_SIZE, chunkPos.z * (float) CHUNK_SIZE);

		glm::vec3 dist = camera.m_position - chunkWorldPos;
		if (glm::dot(dist, dist) > cullRangeSquared)	// removing chunk data from gpu if out of camera.far range
		{
			removeRenderChunk(chunkPos);
			chunk->m_updated = false;
			continue;
		}

		if (!camera.frustumContainsSpheres(chunk->m_bounds, 8, sphereCullRad)) //8 corners
			continue;

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
		if (camera.frustumContainsSpheres(it.second->m_bounds, 8, sphereCullRad)) //8 corners
			m_visibleChunkList.push_back(it.second);
	}
	std::sort(m_visibleChunkList.begin(), m_visibleChunkList.end(), DistanceSort(camera.m_position));
	world.getTileSet()->bind();

	////////////////////////////////////////////////////////////////////////////////////////////

	m_voxelShader.begin();
	m_voxelShader.setUniform1f("u_fogEnd", camera.m_far);
	m_voxelShader.setUniform1f("u_fogStart", camera.m_far * 0.7f);
	m_voxelShader.setUniform3f("u_camPos", camera.m_position);
	m_voxelShader.setUniformMatrix4f("u_mvp", camera.m_combinedMatrix);
	for (const std::shared_ptr<VoxelRenderer::Chunk>& chunk : m_visibleChunkList)
	{
		m_voxelShader.setUniform3f("u_chunkOffset", chunk->m_renderOffset);
		m_renderer.renderChunk(chunk);
	}
	m_voxelShader.end();
}

const std::shared_ptr<VoxelRenderer::Chunk> WorldRenderer::getRenderChunk(const glm::ivec3& pos, const glm::vec3* const bounds)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		const std::shared_ptr<VoxelRenderer::Chunk>& chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE, bounds);
		//printf("Creating: %i %i %i \n", pos.x, pos.y, pos.z);

		m_renderChunks.insert(std::make_pair(pos, chunk));
		m_numLoadedChunks++;
		return chunk;
	}
}

void WorldRenderer::removeRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
	{
		//printf("Removing: %i %i %i \n", pos.x, pos.y, pos.z);
		m_renderChunks.erase(it);
		m_numLoadedChunks--;
	}
}

void WorldRenderer::buildChunk(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world)
{
	const glm::ivec3& chunkPos = chunk->m_pos;
	auto& renderChunk = getRenderChunk(chunkPos, chunk->m_bounds);

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

				//get the 8 surrounding blocks (including pos)
				const VoxelBlock* surroundingBlockData[3][3][3];
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						for (int k = 0; k < 3; ++k)
						{
							glm::ivec3 pos(worldX + (i - 1), worldY + (j - 1), worldZ + (k - 1));
							surroundingBlockData[i][j][k] = &world.getBlock(pos);
						}
					}
				}
				const VoxelBlock* above = surroundingBlockData[1][2][1];
				const VoxelBlock* below = surroundingBlockData[1][0][1];
				const VoxelBlock* front = surroundingBlockData[2][1][1];
				const VoxelBlock* back = surroundingBlockData[0][1][1];
				const VoxelBlock* right = surroundingBlockData[1][1][2];
				const VoxelBlock* left = surroundingBlockData[1][1][0];
				// store the 6 adjacent blocks so we can check if they're solid.
				const VoxelBlock* faceValues[6] = { above, below, left, right, front, back };

				// for every face of the block
				for (int face = 0; face < 6; ++face)
				{
					// if the face is invisible, continue (if no airblock or transparant block touching face)
					if (faceValues[face]->solid)
						continue;

					Color8888 perFaceCols[4];
					unsigned char vertexAO[4];

					for (int vertex = 0; vertex < 4; ++vertex)	//for every vertex
					{
						bool isSolid[3] = { false, false, false };
						int r = 0, g = 0, b = 0, a = 0, numTransparent = 0;

						for (int sample = 0; sample < 3; ++sample)	//3 times per vertex, order should be side, side, corner
						{
							// Get the offset to sample with.
							char sampleXOffset = AO_CHECKS_OFFSET[face][vertex][sample][0];
							char sampleYOffset = AO_CHECKS_OFFSET[face][vertex][sample][1];
							char sampleZOffset = AO_CHECKS_OFFSET[face][vertex][sample][2];

							// get the color for the block at the offset
							// +1 because offset ranges from -1 to 1, and array index goes from 0-2
							const VoxelBlock* blockProperties = surroundingBlockData[sampleXOffset + 1][sampleYOffset + 1][sampleZOffset + 1];

							if (!blockProperties->solid)
							{
								//blend rgba
								r += blockProperties->color.r;
								g += blockProperties->color.g;
								b += blockProperties->color.b;
								a += blockProperties->color.a;
								numTransparent++;
							}
							else
								isSolid[sample] = true;
						}

						//blend rgb
						r += faceValues[face]->color.r;
						g += faceValues[face]->color.g;
						b += faceValues[face]->color.b;
						a += faceValues[face]->color.a;
						numTransparent++;

						//get vertex ao contribution given the touching solid faces.
						vertexAO[vertex] = getAO(isSolid[0], isSolid[1], isSolid[2]);
						// average color of the 4 samples for smooth lighting, dont count solid blocks
						perFaceCols[vertex] = Color8888(r / numTransparent, g / numTransparent, b / numTransparent, a / numTransparent);
					}

					// apply AO to alpha component only for every vertex color of the face
					perFaceCols[0].a = vertexAO[0] > perFaceCols[0].a ? 0 : perFaceCols[0].a - vertexAO[0];
					perFaceCols[1].a = vertexAO[1] > perFaceCols[1].a ? 0 : perFaceCols[1].a - vertexAO[1];
					perFaceCols[2].a = vertexAO[2] > perFaceCols[2].a ? 0 : perFaceCols[2].a - vertexAO[2];
					perFaceCols[3].a = vertexAO[3] > perFaceCols[3].a ? 0 : perFaceCols[3].a - vertexAO[3];

					//get the rendering related properties for this block (texture id's).
					// flip quad to avoid asymmetric color blending 
					bool flipQuad = vertexAO[1] + vertexAO[3] > vertexAO[0] + vertexAO[2];
					renderChunk->addFace((Face) face, x, y, z, properties.renderData.getTextureID((Face) face),
						perFaceCols[0], perFaceCols[3], perFaceCols[1], perFaceCols[2], flipQuad);
				}
			}
		}
	}
	m_renderer.endChunk(renderChunk);
}