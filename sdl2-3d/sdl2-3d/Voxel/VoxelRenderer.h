#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>

#include <assert.h>
#include <memory>

#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "../Engine/Graphics/GL/VertexBuffer.h"
#include "../Engine/Graphics/GL/Shader.h"
#include "../Engine/Graphics/GL/DeferredShader.h"

#include "VoxelBlock.h"
#include "VoxelChunk.h"

class Camera;

class VoxelRenderer
{
private:
	static const unsigned int POSITION_LOC = 0;
	static const unsigned int TEXCOORD_LOC = 1;
	static const unsigned int COLOR_LOC = 2;
	static const unsigned int VERTEX_ID_LOC = 3
		;
	static const unsigned int MAX_FACES_PER_CHUNK = (CHUNK_SIZE / 2)*(CHUNK_SIZE / 2)*(CHUNK_SIZE / 2) * 6;
	static const unsigned int SIZE_BITS = 5; //2^sizebits == chunksize or sqrt(chunkSize) + 1
	static const unsigned int TEXTURE_ID_BITS = 12;

	static_assert(SIZE_BITS * 3 + TEXTURE_ID_BITS <= 32, "Vertex Data must be <= 32 bits");
	static_assert(CHUNK_SIZE == 16, "This class hardcodes an amount of bits to use for x/y/z, dependant on CHUNK_SIZE");//change assert once fixd

	/** block x/y/z/textureid packed into one integer */
	struct VoxelVertex
	{
		VoxelVertex() : x(0), y(0), z(0), textureIdx(0) {};
		VoxelVertex(unsigned int x, unsigned int y, unsigned int z, unsigned int textureIdx)
			: x(x)
			, y(y)
			, z(z)
			, textureIdx(textureIdx)
		{};
		unsigned x : SIZE_BITS;
		unsigned y : SIZE_BITS;
		unsigned z : SIZE_BITS;
		unsigned textureIdx : TEXTURE_ID_BITS;
		unsigned padding : 5;	// unused bits
	};
public:
	friend class WorldRenderer;
	/** Contains the vao/buffers and holds information to draw them, can add faces to this object */
	class Chunk
	{
		friend class VoxelRenderer;
		friend class WorldRenderer;
	public:
		/** 
		Add a cube face at the given position, must be in between VoxelRenderer::beginChunk and VoxelRenderer::endChunk
		- int x/y/z : position within this chunk, must be within 0 to CHUNK_SIZE
		- int textureIdx : texture index witin the TextureArray used for VoxelRenderer::beginRender
		- Color8888 1-4 : color tint for the corners of this face
		*/
		void addFace(Face face, int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4, bool flipQuad = false);
		/** Offset to render with*/
		glm::vec3 m_renderOffset;

		inline unsigned int getNumFaces() const { return m_numFaces; };

	private:
		Chunk(float xOffset, float yOffset, float zOffset, VoxelRenderer& renderer);
		~Chunk();

		VoxelRenderer& m_renderer;
		bool m_begun;
		GLuint m_vao;
		unsigned int m_numFaces;

		VertexBuffer m_pointBuffer;
		VertexBuffer m_colorBuffer;
		VertexBuffer m_indiceBuffer;
	};

	VoxelRenderer();
	VoxelRenderer(const VoxelRenderer& copyMe) = delete;
	~VoxelRenderer();

	const std::shared_ptr<VoxelRenderer::Chunk> createChunk(float xOffset, float yOffset, float zOffset);

	/** Ready a chunk to add faces */
	void beginChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk);
	/** Finish adding faces to a chunk and upload the vertex data */
	void endChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk);

	void renderChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk, GLenum mode = GL_TRIANGLES);

private:
	static void returnChunk(Chunk* chunk);

	bool m_begunChunk;
	bool m_blendEnabled;

	std::vector<glm::vec2> m_texcoordData;
	VertexBuffer m_texcoordBuffer;

	std::vector<Color8888> m_colorData;
	std::vector<VoxelVertex> m_pointData;
	std::vector<unsigned short> m_indiceData;

	std::vector<Chunk*> m_chunkPool;
};