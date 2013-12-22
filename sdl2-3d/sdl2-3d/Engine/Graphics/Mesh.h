#ifndef MESH_H_
#define	MESH_H_

#include <vector>
#include <glm\glm.hpp>

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

    bool loadMesh(const std::string& Filename);
    void render();
    void render(unsigned int numInstances, const glm::mat4* wvpMats, const glm::mat4* worldMats);

private:
	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = normal;
		}
	};

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


    bool initFromScene(const aiScene* scene, const std::string& filename);
	/** Create a mesh from the given vertex data*/
    void initMesh(const aiMesh* aiMesh,
                  std::vector<glm::vec3>& positions,
                  std::vector<glm::vec3>& normals,
                  std::vector<glm::vec2>& texCoords,
                  std::vector<unsigned int>& indices);

    bool initMaterials(const aiScene* scene, const std::string& filename);
    void clear();


	enum Buffers { INDICES, POSITION, NORMAL, TEXCOORD, WORLD_MAT, NUM_BUFFERS };

    GLuint m_vao;
	GLuint m_buffers[NUM_BUFFERS];

    std::vector<MeshEntry> m_entries;
	//TODO: refactor into Material
    std::vector<Texture*> m_textures;
};


#endif	/* MESH_H_ */

