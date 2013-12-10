#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "..\Engine\Graphics\Renderers\UniformBufferStructs.h"
#include "..\Engine\Graphics\TextureArray.h"

#include "VoxelChunk.h"

class Material;
class Camera;

/**
 Imagine a filled chunk with a 16x16x1 slice removed every other block, 
 max amount of faces same that should be possible with proper culling

 //static const unsigned int MAX_FACES_PER_CACHE = CHUNK_SIZE_CUBED / 2;
*/
static const unsigned int MAX_FACES_PER_CACHE = 2048u;

/** Max faces per cache == VoxelChunk::CHUNK_SIZE_CUBED */
class VoxelCache
{
public:
	enum Face
	{
		TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
	};

	/**
	6 of these are used to buffer data from different faces so you can add faces in random order
	(e.g. top, bottom, bottom, right, render, instead of top, top, top, render, bottom bottom etc..
	*/
	struct CacheData
	{
		CacheData() : m_pointIdx(0), m_colorIdx(0), m_begun(false)
		{
			memset(m_points, 0, sizeof(m_points));
			memset(m_colorBits, 0, sizeof(m_colorBits));
		};
		bool m_begun;
		unsigned int m_pointIdx;
		unsigned int m_points[MAX_FACES_PER_CACHE];
		unsigned int m_colorIdx;
		unsigned int m_colorBits[MAX_FACES_PER_CACHE * 4];
	};
	class Cache
	{
		friend class VoxelCache;
	public:
		void addFace(int x, int y, int z, int textureIdx, int color1, int color2, int color3, int color4);
		void setOffset(float xOffset, float yOffset, float zOffset)
		{
			m_xOffset = xOffset;
			m_yOffset = yOffset;
			m_zOffset = zOffset;
		};
		float getXOffset() { return m_xOffset; };
		float getYOffset() { return m_yOffset; };
		float getZOffset() { return m_zOffset; };
	private:
		Cache(GLuint vao, GLuint positionBuffer, Face face, float xOffset, float yOffset, float zOffset)
			: m_vao(vao)
			, m_positionBuffer(positionBuffer)
			, m_face(face)
			, m_amount(0)
			, m_xOffset(xOffset)
			, m_yOffset(yOffset)
			, m_zOffset(zOffset)
			, m_data(0)
		{};
		~Cache() {};

		float m_xOffset, m_yOffset, m_zOffset;
		GLuint m_vao, m_positionBuffer, m_colorBuffer;
		Face m_face;
		unsigned int m_amount;
		VoxelCache::CacheData* m_data;
	};

	VoxelCache();
	VoxelCache(const VoxelCache& copyMe) = delete;
	~VoxelCache();

	Cache* const createCache(Face face, float xOffset = 0.0f, float yOffset = 0.0f, float zOffset = 0.0f);
	void beginCache(VoxelCache::Cache* const cache);
	void endCache(VoxelCache::Cache* const cache);
	void beginRender(const TextureArray* tileSet);
	void renderCache(Cache* const cache, const Camera& camera);
	void finishRender();
	void deleteCache(Cache* const cache);
private:

	void setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset);

	bool m_drawing;
	bool m_blendEnabled;

	GLuint m_shaderId;

	VoxelTransform m_cameraTransform;
	GLuint m_cameraTransformBuffer;
	GLuint m_texCoordBuffer;

	GLuint m_faceVertexBuffer[6];
	CacheData m_cacheData[6];
};

#endif //VOXEL_CACHE_H_