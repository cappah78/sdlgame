#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <glm\glm.hpp>
#include <memory>
#include "VertexBuffer.h"

struct aiMesh;
struct aiScene;
class Texture;

/**
-WIP-
Renderable mesh
*/
class Mesh
{
public:
	typedef unsigned int GLuint;

	static const unsigned int POSITION_LOCATION = 0;
	static const unsigned int TEX_COORD_LOCATION = 1;
	static const unsigned int NORMAL_LOCATION = 2;
	static const unsigned int WORLD_MAT_LOCATION_R1 = 3;
	static const unsigned int WORLD_MAT_LOCATION_R2 = 4;
	static const unsigned int WORLD_MAT_LOCATION_R3 = 5;
	static const unsigned int WORLD_MAT_LOCATION_R4 = 6;

	Mesh();
	Mesh(const std::string& fileName);
	~Mesh();

	void loadMesh(const std::string& Filename);
	//void render();
	//void render(unsigned int numInstances, const glm::mat4* wvpMats, const glm::mat4* worldMats);

private:

	struct MeshEntry {
		static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;

		MeshEntry()
			: numIndices(0)
			, baseVertex(0)
			, baseIndex(0)
			, materialIndex(INVALID_MATERIAL)
		{};

		unsigned int numIndices;
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int materialIndex;
	};

	void initVertexBuffers(const aiScene* scene);
	bool initMaterials(const aiScene* scene, const std::string& filename);

	std::unique_ptr<VertexBuffer> m_indiceBuffer;
	std::unique_ptr<VertexBuffer> m_positionBuffer;
	std::unique_ptr<VertexBuffer> m_normalBuffer;
	std::unique_ptr<VertexBuffer> m_tangentBuffer;
	std::unique_ptr<VertexBuffer> m_bitangentBuffer;
	std::vector<std::unique_ptr<VertexBuffer>> m_texcoordBuffers;
	std::vector<std::unique_ptr<VertexBuffer>> m_colorBuffers;

	std::vector<MeshEntry> m_entries;
	//TODO: refactor into Material
	std::vector<Texture*> m_textures;
};

#endif MESH_H_
