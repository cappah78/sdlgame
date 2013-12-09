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
	enum Face
	{
		TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
	};

	class Cache
	{
		friend class VoxelCache;
	public:
		float m_xOffset, m_yOffset, m_zOffset;

		void addFace(int x, int y, int z, int textureIdx, float color1, float color2, float color3, float color4);

	private:
		Cache(GLuint vao, unsigned int size, GLuint positionBuffer,
			Face face,
			float xOffset, float yOffset, float zOffset)
			: m_vao(vao)
			, m_positionBuffer(positionBuffer)
			, m_face(face)
			, m_size(size)
			, m_pointIdx(0)
			, m_xOffset(xOffset)
			, m_yOffset(yOffset)
			, m_zOffset(zOffset)
		{
			m_points = new unsigned int[size];
		};
		~Cache()
		{
			delete[] m_points;
		};

		// cache specific data
		GLuint m_vao, m_positionBuffer;
		//type of this cache
		Face m_face;
		//max amount of points
		unsigned int m_size;
		//index of last added point
		unsigned int m_pointIdx;
		//array containing the positions
		unsigned int* m_points;
	};

	VoxelCache();
	VoxelCache(const VoxelCache& copyMe) = delete;
	~VoxelCache();

	Cache* const createCache(Face face, unsigned int size, float xOffset = 0.0f, float yOffset = 0.0f, float zOffset = 0.0f);

	void beginRender();
	void renderCache(Cache* const cache, const TextureArray* tileSet, const Camera& camera);
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
	GLuint m_topFaceVertexBuffer;
	GLuint m_bottomFaceVertexBuffer;
	GLuint m_leftFaceVertexBuffer;
	GLuint m_rightFaceVertexBuffer;
	GLuint m_frontFaceVertexBuffer;
	GLuint m_backFaceVertexBuffer;
};

#endif //VOXEL_CACHE_H_