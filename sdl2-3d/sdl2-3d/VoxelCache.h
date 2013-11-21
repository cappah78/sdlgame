#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include "UniformBufferStructs.h"

#include <gl\glew.h>
#include <glm\glm.hpp>

class Material;
class Camera;

class VoxelCache
{
public:
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

	Cache* const createCache(Face face, float xOffset, float yOffset, float zOffset);

	void beginCache(Cache* const cache, Face face, float xOffset, float yOffset, float zOffset);
	void beginCache(Face face, float xOffset, float yOffset, float zOffset);
	Cache* const endCache();

	void addFace(int x, int y, int z, const Material& material);

	void renderCache(Cache* const cache, const Camera& camera);
	void deleteCache(Cache* const cache);
private:
	void setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset);
	void flush();
	void swapMaterial(const Material* const material);

	unsigned int m_sizeInFaces;
	const Material* m_currentMaterial;
	bool m_drawing;
	bool m_blendEnabled;

	VertexTransform m_vertexTransform;
	GLuint m_vertexTransformBuffer;

	GLuint m_shaderId;
	GLuint m_pointBuffer;

	Cache* m_currentCache;

	unsigned int m_pointIdx;
	unsigned int* m_points;
};

#endif //VOXEL_CACHE_H_