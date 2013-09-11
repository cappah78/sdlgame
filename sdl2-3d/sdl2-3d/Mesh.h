/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MESH_H_
#define	MESH_H_

#include <vector>
#include <glm\glm.hpp>

struct aiMesh;
struct aiScene;
class Texture;

typedef unsigned int GLuint;

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};


class Mesh
{
public:
    Mesh();
	Mesh(const std::string& fileName);

    ~Mesh();

    bool loadMesh(const std::string& Filename);

    void render();
	
    void render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

private:
    bool initFromScene(const aiScene* pScene, const std::string& Filename);
    void initMesh(const aiMesh* paiMesh,
                  std::vector<glm::vec3>& Positions,
                  std::vector<glm::vec3>& Normals,
                  std::vector<glm::vec2>& TexCoords,
                  std::vector<unsigned int>& Indices);

    bool initMaterials(const aiScene* pScene, const std::string& Filename);
    void clear();

#define INVALID_MATERIAL 0xFFFFFFFF
   
#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5

    GLuint m_VAO;
    GLuint m_Buffers[6];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
		unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
};


#endif	/* MESH_H_ */

