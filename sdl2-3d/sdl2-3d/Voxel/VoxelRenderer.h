#pragma once

#include "../Engine/Graphics/Model/IGraphicsProvider.h"
#include "VoxelChunk.h"

#include <assert.h>
#include <memory>
#include <gl/glew.h>

class Camera;

class VoxelRenderer
{
public:
	friend class WorldRenderer;

	class Chunk
	{
	public:
		friend class VoxelRenderer;
		friend class WorldRenderer;

		void addFace(Face face, int x, int y, int z, int textureIdx, unsigned char vertex1AO, unsigned char vertex2AO, unsigned char vertex3AO, unsigned char vertex4AO, bool flipQuad);
		inline unsigned int getNumFaces() const { return m_numFaces; };

		glm::vec3 m_renderOffset;

	private:
		Chunk(float xOffset, float yOffset, float zOffset, VoxelRenderer& renderer);
		~Chunk();

		VoxelRenderer& m_renderer;
		unsigned int m_numFaces;
		bool m_begun;

		std::unique_ptr<IStateBuffer> m_state;
		std::unique_ptr<IVertexBuffer> m_pointBuffer;
		std::unique_ptr<IIndiceBuffer> m_indiceBuffer;
	};

	VoxelRenderer();
	VoxelRenderer(const VoxelRenderer& copyMe) = delete;
	~VoxelRenderer();

	const std::shared_ptr<VoxelRenderer::Chunk> createChunk(float xOffset, float yOffset, float zOffset);
	void beginChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk);
	void endChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk);
	void renderChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk, GLenum mode = GL_TRIANGLES);

private:

	static void returnChunk(Chunk* chunk);

	static const unsigned int MAX_FACES_PER_CHUNK = (CHUNK_SIZE / 2) * (CHUNK_SIZE / 2) * (CHUNK_SIZE / 2) * 6;

	static const unsigned int POSITION_LOC = 0;
	static const unsigned int TEXCOORD_LOC = 1;

	static const unsigned int SIZE_BITS = 5; // number of bits for position per axis
	static const unsigned int TEXTURE_ID_BITS = 10;
	static_assert(SIZE_BITS * 3 + TEXTURE_ID_BITS <= 32, "Vertex Data must be <= 32 bits");

	/** block x/y/z/textureid packed into one integer */
	struct VoxelVertex
	{
		VoxelVertex() : x(0), y(0), z(0), textureIdx(0) {};
		VoxelVertex(unsigned int x, unsigned int y, unsigned int z, unsigned int textureIdx, unsigned int ao)
			: x(x)
			, y(y)
			, z(z)
			, textureIdx(textureIdx)
			, ao(ao)
		{};
		unsigned x : SIZE_BITS;
		unsigned y : SIZE_BITS;
		unsigned z : SIZE_BITS;
		unsigned textureIdx : TEXTURE_ID_BITS;
		unsigned ao : 7;
	};

	bool m_begunChunk;
	bool m_blendEnabled;

	std::vector<glm::vec2> m_texcoordData;
	std::unique_ptr<IVertexBuffer> m_texcoordBuffer;

	std::vector<VoxelVertex> m_pointData;
	std::vector<unsigned short> m_indiceData;
	std::vector<Chunk*> m_chunkPool;
};