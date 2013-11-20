#ifndef VOXEL_CACHE_H_
#define VOXEL_CACHE_H_

#include "UniformBufferStructs.h"

#include <gl\glew.h>
#include <glm\glm.hpp>

class Material;
class Camera;

typedef unsigned int CacheID;

class VoxelCache
{
public:
	VoxelCache(unsigned int sizeInFaces = 2000);
	VoxelCache(const VoxelCache& copyMe);
	~VoxelCache();

	CacheID begin(const Camera& camera);
	void begin(const Camera& camera, CacheID cacheID);

	void end();
private:
	void flush();
	void swapMaterial(const Material* const material);

	unsigned int m_sizeInFaces;
	const Material* m_currentMaterial;
	bool m_drawing;
	bool m_blendEnabled;
	unsigned int m_drawCalls;

	CameraTransform m_cameraTransform;
	GLuint m_cameraTransformBuffer;

	GLuint m_shaderId;
	GLuint m_vao;
	GLuint m_vertexBuffer;
	GLuint m_texCoordBuffer;
	GLuint m_indexBuffer;

	unsigned int m_vertexIdx;
	float* m_vertices;
	unsigned int m_texCoordIdx;
	float* m_texCoords;
	unsigned short* m_indices;
};

#endif //VOXEL_CACHE_H_