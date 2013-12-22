#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "..\Engine\Graphics\TextureArray.h"
#include "../Engine/Graphics/Color8888.h"

#include "VoxelChunk.h"

class Material;
class Camera;

/**
 Imagine a filled chunk with a 16x16x1 slice removed every other block, 
 max amount of faces same that should be possible with proper culling

 //static const unsigned int MAX_FACES_PER_CACHE = CHUNK_SIZE_CUBED / 2;
*/
static const unsigned int MAX_FACES_PER_CACHE = 2048u;

/** Manages and renders 'Cache' objects which are a collection of faces */
class VoxelRenderer
{
private:
	typedef unsigned int GLuint;

	/** Is used internally to add faces and update vertex buffers */
	struct PerFaceVertexData
	{
		/** position counter */
		unsigned int m_pointIdx;
		/** positions, one per face*/
		unsigned int m_points[MAX_FACES_PER_CACHE];
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

	/** Contains the vao/buffers and holds information to draw it, can add faces to this object */
	class Cache
	{
		friend class VoxelRenderer;
	public:
		void addFace(Face face, int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4);
		float m_xOffset, m_yOffset, m_zOffset;
	private:
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

	void beginCache(VoxelRenderer::Cache* const cache);
	void endCache(VoxelRenderer::Cache* const cache);

	void beginRender(const TextureArray* tileSet);
	void renderCache(Cache* const cache, const Camera& camera);
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

#endif //VOXEL_CACHE_H_