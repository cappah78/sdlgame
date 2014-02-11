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
	, m_geometryShader("Assets/Shaders/geometry.vert", NULL, "Assets/Shaders/geometry.frag")
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
			chunk->m_shouldUpdate = true;
			continue;
		}

		//if (!Frustum::aabbInFrustum(chunkWorldPos, glm::vec3(8), frustumCullMat))
		//	continue;

		if (Game::getSDLTicks() - startTicks > MAX_MS_CHUNK_GEN_PER_FRAME)	//smooth out over multiple frames
			continue;	//continue, not break because we still want to remove chunks that are out of range even when past max time
		if (!chunk->m_shouldUpdate)
			continue;
		chunk->m_shouldUpdate = false;

		//buildChunk(chunk, world);
		buildChunkWithAO(chunk, world);
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
						0, 0, 0, 0, false);
				}
			}
		}
	}
	m_renderer.endChunk(renderChunk);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////Alternative with simple AO applied to vertex color attribute//////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO: refactor so offsets are calculated programmatically instead of from this array.
//face, vertex, offsetvec	
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

static const unsigned char AO_STRENGTH = 80;
inline unsigned char getAO(bool side, bool side2, bool corner)
{
	return side && side2 ? 2 * AO_STRENGTH : (side + corner + side2) * AO_STRENGTH;
}

void DeferredWorldRenderer::buildChunkWithAO(const std::unique_ptr<VoxelChunk>& chunk, VoxelWorld& world)
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
					renderChunk->addFace((Face) face, x, y, z, properties.getTextureID((Face) face),
						vertexAO[0], vertexAO[3], vertexAO[1], vertexAO[2], flipQuad);
				}
			}
		}
	}
	m_renderer.endChunk(renderChunk);
}
