#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "..\Engine\Graphics\TextureArray.h"
#include "../Engine/Graphics/Color8888.h"

#include "VoxelChunk.h"
#include <glm\glm.hpp>


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
private:
	/**
	6 of these are used to buffer data from different faces so you can add faces in random order
	(e.g. top, bottom, bottom, right, render, instead of top, top, top, render, bottom bottom etc..
	A bit ugly, but the convenience is worth it.
	*/
	struct CacheData
	{
		CacheData() : m_pointIdx(0), m_colorIdx(0), m_begun(false) {};
		bool m_begun;
		unsigned int m_pointIdx;
		unsigned int m_points[MAX_FACES_PER_CACHE];
		unsigned int m_colorIdx;
		Color8888 m_colorBits[MAX_FACES_PER_CACHE * 4];
	};

public:
	enum Face
	{
		TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
	};

	/** Contains the vao/buffers and holds information to draw it, can add faces to this object */
	class Cache
	{
		friend class VoxelCache;
	public:
		void addFace(int x, int y, int z, int textureIdx, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4);
		Face getFace() { return m_face; };
		unsigned int getNumFaces() { return m_numFaces; };
	private:
		Cache(GLuint vao, GLuint positionBuffer, GLuint colorBuffer, Face face)
			: m_vao(vao)
			, m_positionBuffer(positionBuffer)
			, m_colorBuffer(colorBuffer)
			, m_face(face)
			, m_numFaces(0)
			, m_data(0)
		{};
		~Cache() {};

		GLuint m_vao, m_positionBuffer, m_colorBuffer;
		Face m_face;
		unsigned int m_numFaces;
		VoxelCache::CacheData* m_data;
	};

	VoxelCache();
	VoxelCache(const VoxelCache& copyMe) = delete;
	~VoxelCache();

	Cache* const createCache(Face face);
	void beginCache(VoxelCache::Cache* const cache);
	void endCache(VoxelCache::Cache* const cache);
	void beginRender(const TextureArray* tileSet);
	void renderCache(Cache* const cache, const Camera& camera);
	void finishRender();
	void deleteCache(Cache* const cache);
	void setMVPUniform(glm::mat4 mvpMatrix);
	void setNormalUniform(glm::vec3 normal);
private:
	bool m_drawing;
	bool m_blendEnabled;

	GLuint m_shaderId;

	GLuint m_mvpUniformLoc;
	GLuint m_normalUniformLoc;

	GLuint m_texCoordBuffer;
	GLuint m_faceVertexBuffer[6];
	CacheData m_cacheData[6];
};

#endif //VOXEL_CACHE_H_