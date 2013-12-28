#include "WorldRenderer.h"

#include "VoxelWorld.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "PropertyManager.h"

WorldRenderer::WorldRenderer()
{
	
}

WorldRenderer::~WorldRenderer()
{

}

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

void WorldRenderer::render(const VoxelWorld& world, const Camera& camera)
{
	const ChunkManager::ChunkMap& chunks = world.getChunks();
	
	for (auto it : chunks)
	{
		VoxelChunk* chunk = it.second;
		const glm::ivec3& chunkPos = chunk->m_pos;

		if (chunk->m_updated)
			continue;
		chunk->m_updated = true;
		const std::shared_ptr<VoxelRenderer::Chunk>& renderChunk = getRenderChunk(chunkPos);
		m_renderer.beginChunk(renderChunk);

		BlockID* blocks = chunk->getBlocks();
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					unsigned int blockIdx = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z;
					BlockID id = blocks[blockIdx];
					if (id == 0)
						continue;

					int worldX = chunkPos.x * CHUNK_SIZE + x;
					int worldY = chunkPos.y * CHUNK_SIZE + y;
					int worldZ = chunkPos.z * CHUNK_SIZE + z;

					//get the 9 surrounding blocks
					BlockIDColor idColors[3][3][3];
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							for (int k = 0; k < 3; ++k)
							{
								glm::ivec3 pos(worldX + (i - 1), worldY + (j - 1), worldZ + (k - 1));
								idColors[i][j][k] = world.getBlockIDColor(pos);
							}
						}
					}

					BlockIDColor above = idColors[1][2][1];
					BlockIDColor below = idColors[1][0][1];
					BlockIDColor front = idColors[2][1][1];
					BlockIDColor back = idColors[0][1][1];
					BlockIDColor right = idColors[1][1][2];
					BlockIDColor left = idColors[1][1][0];
					// store the 6 adjacent blocks so we can check if they're solid blocks.
					BlockIDColor faceValues[6] = { above, below, left, right, front, back };

					// for every face of the block
					for (int face = 0; face < 6; ++face)
					{
						// if the face is invisible, continue (if no airblock or transparant block touching face)
						// alpha 0 is solid block, id 0 is air
						if (faceValues[face].id != 0 && faceValues[face].color.a == 0)	
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
								BlockIDColor idCol = idColors[sampleXOffset + 1][sampleYOffset + 1][sampleZOffset + 1];	

								//blend rgb
								r += idCol.color.r;
								g += idCol.color.g;
								b += idCol.color.b;

								if (idCol.color.a != 0) // alpha 0 is solid block
								{
									// blend a
									a += idCol.color.a;
									numTransparent++;
								}
								else
									isSolid[sample] = true;
							}

							//blend rgb
							r += faceValues[face].color.r;
							g += faceValues[face].color.g;
							b += faceValues[face].color.b;
							a += faceValues[face].color.a;
							numTransparent++;

							//get vertex ao contribution given the touching solid faces.
							vertexAO[vertex] = getAO(isSolid[0], isSolid[1], isSolid[2]);
							// average color of the 4 samples for smooth lighting, dont average alpha with solid blocks (those have a == 0)
							perFaceCols[vertex] = Color8888(r / 4, g / 4, b / 4, (a / numTransparent));
						}

						// apply AO to alpha component only for every vertex color of the face
						perFaceCols[0].a = vertexAO[0] > perFaceCols[0].a ? 0 : perFaceCols[0].a - vertexAO[0];
						perFaceCols[1].a = vertexAO[1] > perFaceCols[1].a ? 0 : perFaceCols[1].a - vertexAO[1];
						perFaceCols[2].a = vertexAO[2] > perFaceCols[2].a ? 0 : perFaceCols[2].a - vertexAO[2];
						perFaceCols[3].a = vertexAO[3] > perFaceCols[3].a ? 0 : perFaceCols[3].a - vertexAO[3];

						//get the rendering related properties for this block (texture id's).
						const PropertyManager& propertyManager = world.getPropertyManager();
						BlockRenderData properties = propertyManager.getBlockRenderData(id);

						// flip quad to avoid asymmetric color blending 
						bool flipQuad = vertexAO[1] + vertexAO[3] > vertexAO[0] + vertexAO[2];
						renderChunk->addFace((VoxelRenderer::Face) face, x, y, z, properties.getTextureID((VoxelRenderer::Face) face),
							perFaceCols[0], perFaceCols[3], perFaceCols[1], perFaceCols[2], flipQuad);
					}
				}
			}
		}
		m_renderer.endChunk(renderChunk);
	}

	m_renderer.beginRender(world.getTileSet());
	for (auto it : m_renderChunks)
	{
		m_renderer.renderChunk(it.second, camera);
	}
	m_renderer.endRender();
}

const std::shared_ptr<VoxelRenderer::Chunk> WorldRenderer::getRenderChunk(const glm::ivec3& pos)
{
	auto it = m_renderChunks.find(pos);
	if (it != m_renderChunks.end())
		return it->second;
	else
	{
		//printf("createchunk: %i, %i, %i \n", pos.x, pos.y, pos.z);
		const std::shared_ptr<VoxelRenderer::Chunk>& chunk = m_renderer.createChunk((float) pos.x * CHUNK_SIZE, (float) pos.y * CHUNK_SIZE, (float) pos.z * CHUNK_SIZE);
		m_renderChunks.insert(std::make_pair(pos, chunk));
		return chunk;
	}
}