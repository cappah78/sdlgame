#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "..\Engine\Graphics\Renderers\UniformBufferStructs.h"
#include "..\Engine\Graphics\TextureArray.h"

class Material;
class Camera;

class VoxelCache
{
public:

	enum Occlusion
	{
		AO_UP = 1, AO_UP_RIGHT = 2, AO_RIGHT = 4, AO_DOWN_RIGHT = 8, AO_DOWN = 16, AO_DOWN_LEFT = 32, AO_LEFT = 64, AO_UP_LEFT = 128
	};

	enum Face
	{
		TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT
	};

	class Cache
	{
		friend class VoxelCache;
	private:
		Cache(GLuint vao, GLuint pointBuffer,
			Face face,
			float xOffset, float yOffset, float zOffset)
			: m_vao(vao)
			, m_pointBuffer(pointBuffer)
			, m_face(face)
			, m_amount(0)
			, m_xOffset(xOffset)
			, m_yOffset(yOffset)
			, m_zOffset(zOffset)
		{};

		GLuint m_vao, m_pointBuffer;
		Face m_face;
		unsigned int m_amount;
	public:
		float m_xOffset, m_yOffset, m_zOffset;
	};

	VoxelCache(unsigned int sizeInFaces = 40000);
	VoxelCache(const VoxelCache& copyMe);
	~VoxelCache();

	Cache* const createCache(Face face, float xOffset = 0.0f, float yOffset = 0.0f, float zOffset = 0.0f);

	void beginCache(Cache* const cache, Face face, float xOffset, float yOffset, float zOffset);
	void beginCache(Face face, float xOffset, float yOffset, float zOffset);
	Cache* const endCache();

	void addFace(int x, int y, int z, int textureIdx, unsigned char occlusionBits);

	void beginRender();
	void renderCache(Cache* const cache, const TextureArray* tileSet, const Camera& camera);
	void finishRender();
	void deleteCache(Cache* const cache);
private:
	void setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset);

	unsigned int m_sizeInFaces;
	bool m_buildingCache;
	bool m_drawing;
	bool m_blendEnabled;

	VoxelTransform m_cameraTransform;
	GLuint m_cameraTransformBuffer;

	GLuint m_vertexBuffer;
	GLuint m_cornerIndexBuffer;
	GLuint m_texCoordBuffer;
	GLuint m_positionBuffer;

	GLuint m_shaderId;

	Cache* m_currentCache;

	unsigned int m_pointIdx;
	unsigned int* m_points;
};

#endif //VOXEL_CACHE_H_