#ifndef MESH_H_
#define	MESH_H_

#include <vector>
#include <glm\glm.hpp>

struct aiMesh;
struct aiScene;
class Texture;

typedef unsigned int GLuint;

class Mesh
{
public:
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

	enum BufferIndex { INDEX_BUFFER = 0, POS_VB = 1, NORMAL_VB = 2, TEXCOORD_VB = 3, WVP_MAT_VB = 4, WORLD_MAT_VB = 5 };
	static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;

    bool initFromScene(const aiScene* scene, const std::string& filename);
    void initMesh(const aiMesh* aiMesh,
                  std::vector<glm::vec3>& positions,
                  std::vector<glm::vec3>& normals,
                  std::vector<glm::vec2>& texCoords,
                  std::vector<unsigned int>& indices);

    bool initMaterials(const aiScene* scene, const std::string& filename);
    void clear();



    GLuint m_vao;
    GLuint m_buffers[6];

    
    
    std::vector<MeshEntry> m_entries;
    std::vector<Texture*> m_textures;
};


#endif	/* MESH_H_ */

