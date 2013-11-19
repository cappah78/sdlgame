#ifndef VOXEL_BATCH_H_
#define VOXEL_BATCH_H_

#include "UniformBufferStructs.h"

#include <gl\glew.h>
#include <glm\glm.hpp>



class Material;
class Camera;

class VoxelBatch
{
public:
	static const int FACE_SIZE = 3 * 4;

	VoxelBatch(unsigned int sizeInFaces = 2000);
	VoxelBatch(const VoxelBatch& copyMe);
	~VoxelBatch();

	void begin(const Camera& camera);
	void end();

	enum Face
	{
		UP, DOWN, FRONT, BACK, LEFT, RIGHT
	};

	void renderFace(const Face face, int x, int y, int z, const Material& material);
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

#endif //VOXEL_BATCH_H_