#ifndef VOXEL_RENDERER_H_
#define VOXEL_RENDERER_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include <assert.h>
#include <memory>


#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "../Engine/Graphics/GL/VertexBuffer.h"

#include "VoxelChunk.h"

class Camera;

/** 
Efficiently renders textured/colored cubes minecraft style.
Uses VoxelRenderer::Chunk objects to store, render and add faces.

General usage:
VoxelRenderer::Chunk* chunk = voxelRenderer.createChunk(xOffset, yOffset, zOffset);
voxelRenderer.beginChunk(chunk);
// for every visible face that fits within this cache (size is VoxelChunk::CHUNK_SIZE)
chunk->addFace(Face::TOP, x, y, z, textureIdx, color, color, color, color);

voxelRenderer.beginRender(textureArray);
voxelRenderer.renderChunk(chunk, camera);
voxelRenderer.endRender();

Implementation details:
A single index and texcoord buffer is shared by all caches. Only a single vertex and a single color
array will be used to construct the data on the gpu, its shared by all caches.
*/
class VoxelRenderer
{
private:
	static const unsigned int POSITION_LOC = 0;
	static const unsigned int TEXCOORD_LOC = 1;
	static const unsigned int COLOR_LOC = 2;

	static const unsigned int MAX_FACES_PER_CACHE = 2048 * 6; // == CHUNK_SIZE_CUBED / 2;

	static_assert(CHUNK_SIZE == 16, "This class hardcodes an amount of bits to use for x/y/z, dependant on CHUNK_SIZE");//change assert once fixd
	static const unsigned int SIZE_BITS = 5; //2^sizebits == chunksize or sqrt(chunkSize) + 1
	static const unsigned int TEXTURE_ID_BITS = 12;
	static_assert(SIZE_BITS * 3 + TEXTURE_ID_BITS <= 32, "Vertex Data must be <= 32 bits");

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
	enum Face
	{
		TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
	};

	/** Contains the vao/buffers and holds information to draw them, can add faces to this object */
	class Chunk
	{
		friend class VoxelRenderer;
	public:
		/** 
		Add a cube face at the given position, must be in between VoxelRenderer::beginChunk and VoxelRenderer::endChunk
		- int x/y/z : position within this cache, must be within 0 to CHUNK_SIZE
		- int textureIdx : texture index witin the TextureArray used for VoxelRenderer::beginRender
		- Color8888 1-4 : color tint for the corners of this face
		*/
		void addFace(Face face, int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4);
		/** Offset to render with*/
		float m_xOffset, m_yOffset, m_zOffset;

		~Chunk();

	private:

		Chunk(float xOffset, float yOffset, float zOffset)
			: m_colorBuffer(0)
			, m_pointBuffer(0)
			, m_xOffset(xOffset)
			, m_yOffset(yOffset)
			, m_zOffset(zOffset)
			, m_numFaces(0)
			, m_begun(false)
		{};

		bool m_begun;
		GLuint m_vao;
		unsigned int m_numFaces;

		VertexBuffer<VoxelVertex> m_pointBuffer;
		VertexBuffer<Color8888> m_colorBuffer;
	};

	VoxelRenderer();
	VoxelRenderer(const VoxelRenderer& copyMe) = delete;
	~VoxelRenderer();

	const std::shared_ptr<VoxelRenderer::Chunk> createChunk(float xOffset = 0, float yOffset = 0, float zOffset = 0);

	/** Ready a chunk to add faces */
	void beginChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk);
	/** Finish adding faces to a chunk and upload the vertex data */
	void endChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk);

	/** Prepare the renderer for rendering using the given tileset */
	void beginRender(const TextureArray* tileSet);
	/** Draw the given chunk with its offset, must be in between VoxelRenderer::beginRender and VoxelRenderer::endRender */
	void renderChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk, const Camera& camera);
	/** Finish rendering */
	void endRender();

private:
	void setMVPUniform(glm::mat4 mvpMatrix);
	void setNormalUniform(glm::vec3 normal);

	bool m_begunCache;
	bool m_begunRender;

	bool m_blendEnabled;

	GLuint m_shaderId;

	GLuint m_mvpUniformLoc;
	GLuint m_normalUniformLoc;
	GLuint m_offsetUniformLoc;

	VertexBuffer<glm::vec2> m_texcoordBuffer;
	VertexBuffer<unsigned short> m_indiceBuffer;

	std::vector<Color8888> m_colorData;
	std::vector<VoxelVertex> m_pointData;
};

#endif //VOXEL_RENDERER_H_