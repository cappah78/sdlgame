#ifndef VOXEL_RENDERER_H_
#define VOXEL_RENDERER_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include <assert.h>

#include "../Engine/Graphics/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"

#include "VoxelChunk.h"

class Camera;

static const unsigned int MAX_FACES_PER_CACHE = 2048u; // == CHUNK_SIZE_CUBED / 2;
static_assert(CHUNK_SIZE == 16, "This class requires chunk size of 16 to function, change once fixed");

static const unsigned int SIZE_BITS = 4; //2^sizebits == chunksize or sqrt(chunkSize)
static const unsigned int TEXTURE_ID_BITS = 12;
static_assert(SIZE_BITS * 3 + TEXTURE_ID_BITS <= 32, "Face Point Data must be <= 32 bits");

/** 
Efficiently renders textured/colored cubes minecraft style.
Uses VoxelRenderer::Cache objects to store, render and add faces.

General usage:
VoxelRenderer::Cache* cache = voxelRenderer.createCache(xOffset, yOffset, zOffset);
voxelRenderer.beginCache(cache);
// for every visible face that fits within this cache (size is VoxelChunk::CHUNK_SIZE)
cache->addFace(Face::TOP, x, y, z, textureIdx, color, color, color, color);

voxelRenderer.beginRender(textureArray);
voxelRenderer.renderCache(cache, camera);
voxelRenderer.endRender();
*/
class VoxelRenderer
{
private:
	typedef unsigned int GLuint;

	struct FacePointData
	{
		FacePointData() {};
		FacePointData(unsigned int x, unsigned int y, unsigned int z, unsigned int textureIdx)
			: x(x)
			, y(y)
			, z(z)
			, textureIdx(textureIdx)
		{};
		unsigned x : SIZE_BITS;
		unsigned y : SIZE_BITS;
		unsigned z : SIZE_BITS;
		unsigned textureIdx : TEXTURE_ID_BITS;
		unsigned padding : 32 - SIZE_BITS * 3 - TEXTURE_ID_BITS;	// unused bits
	};

	/** Is used internally to add faces and update vertex buffers */
	struct PerFaceVertexData
	{
		/** position counter */
		unsigned int m_pointIdx;
		/** positions, one per face*/
		FacePointData m_points[MAX_FACES_PER_CACHE];
		/** color counter*/
		unsigned int m_colorIdx;
		/** colors, 4 per face */
		Color8888 m_colorBits[MAX_FACES_PER_CACHE * 4];
	};

	struct PerFaceBufferData
	{
		GLuint m_vao, m_positionBuffer, m_colorBuffer;
		// amount of faces to render
		unsigned int m_numFaces;
	};

public:
	enum Face
	{
		TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
	};

	/** Contains the vao/buffers and holds information to draw them, can add faces to this object */
	class Cache
	{
		friend class VoxelRenderer;
	public:
		/** 
		Add a cube face at the given position, must be in between VoxelRenderer::beginCache and VoxelRenderer::endCache 
		- int x/y/z : position within this cache, must be within 0 to CHUNK_SIZE
		- int textureIdx : texture index witin the TextureArray used for VoxelRenderer::beginRender
		- Color8888 1-4 : color tint for the corners of this face
		*/
		void addFace(Face face, int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4);
		/** Offset to render with*/
		float m_xOffset, m_yOffset, m_zOffset;

	private:
		/** This object is managed by VoxelRenderer::createCache and VoxelRenderer::deleteCache */
		Cache(float xOffset, float yOffset, float zOffset) 
			: m_data(NULL) 
			, m_xOffset(xOffset)
			, m_yOffset(yOffset)
			, m_zOffset(zOffset)
		{};
		~Cache() {};

		PerFaceBufferData m_faceBuffers[6];
		// pointer to array of size 6
		PerFaceVertexData* m_data;
	};

	VoxelRenderer();
	VoxelRenderer(const VoxelRenderer& copyMe) = delete;
	~VoxelRenderer();

	Cache* const createCache(float xOffset = 0, float yOffset = 0, float zOffset = 0);
	void deleteCache(Cache* const cache);

	/** Ready a cache to add faces */
	void beginCache(VoxelRenderer::Cache* const cache);
	/** Finish adding faces to a cache and upload the vertex data */
	void endCache(VoxelRenderer::Cache* const cache);

	/** Prepare the renderer for rendering using the given tileset */
	void beginRender(const TextureArray* tileSet);
	/** Draw the given cache with its offset, must be in between VoxelRenderer::beginRender and VoxelRenderer::endRender */
	void renderCache(Cache* const cache, const Camera& camera);
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

	/** One for each face of a cube */
	GLuint m_faceVertexBuffer[6];
	GLuint m_texCoordBuffer;

	/** Shared by all caches to add faces before updating the buffers */
	PerFaceVertexData m_data[6];
};

#endif //VOXEL_RENDERER_H_