#include "mesh.h"
#include "Texture.h"

#include <assert.h>
#include <gl\glew.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define GLCheckError() (glGetError() == GL_NO_ERROR)

static const aiVector3D ZERO_VEC(0.0f, 0.0f, 0.0f);

Mesh::Mesh() : m_vao(0)
{
    ZERO_MEM(m_buffers);
}

Mesh::Mesh(const std::string& fileName) : m_vao(0)
{
    ZERO_MEM(m_buffers);
	loadMesh(fileName);
}

Mesh::~Mesh()
{
    clear();
}

void Mesh::clear()
{
    for (unsigned int i = 0 ; i < m_textures.size() ; i++)
        SAFE_DELETE(m_textures[i]);

    if (m_buffers[0] != 0)
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
       
    if (m_vao != 0) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
    }
}

bool Mesh::loadMesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    clear();
 
    // Create the VAO
    glGenVertexArrays(1, &m_vao);   
	glBindVertexArray(m_vao);
    
    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

    bool Ret = false;

    const aiScene* scene = aiImportFile(filename.c_str(), aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_GenSmoothNormals | aiPostProcessSteps::aiProcess_FlipUVs);

	if (scene)
		Ret = initFromScene(scene, filename);
    else
        printf("Error parsing '%s'\n", filename.c_str());

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);	

    return Ret;
}

bool Mesh::initFromScene(const aiScene* scene, const std::string& filename)
{  
	m_entries.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

    unsigned int numVertices = 0;
    unsigned int numIndices = 0;
    
    // Count the number of vertices and indices
    for (unsigned int i = 0 ; i < m_entries.size() ; i++) 
	{
		m_entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
		m_entries[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
        m_entries[i].baseVertex = numVertices;
        m_entries[i].baseIndex = numIndices;
        
		numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices  += m_entries[i].numIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    indices.reserve(numIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_entries.size() ; i++) 
	{
        const aiMesh* paiMesh = scene->mMeshes[i];
        initMesh(paiMesh, positions, normals, texCoords, indices);
    }

    if (!initMaterials(scene, filename)) 
        return false;

    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_MAT]);
    
	//put every row of the matrix in a seperate vec4 attribute
    for (unsigned int i = 0; i < 4 ; i++) {
        glEnableVertexAttribArray(WORLD_MAT_LOCATION_R1 + i);
		glVertexAttribPointer(WORLD_MAT_LOCATION_R1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*) (sizeof(GLfloat) * 4 * i));
		glVertexAttribDivisor(WORLD_MAT_LOCATION_R1 + i, 1);
    }
    
    return GLCheckError();
}

void Mesh::initMesh(const aiMesh* aiMesh,
	std::vector<glm::vec3>& positions,
	std::vector<glm::vec3>& normals,
	std::vector<glm::vec2>& texCoords,
	std::vector<unsigned int>& indices)
{    
    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) 
	{
        const aiVector3D* pos = &(aiMesh->mVertices[i]);
        const aiVector3D* normal = &(aiMesh->mNormals[i]);
		const aiVector3D* texCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &ZERO_VEC;

        positions.push_back(glm::vec3(pos->x, pos->y, pos->z));
        normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
        texCoords.push_back(glm::vec2(texCoord->x, texCoord->y));
    }

    // Populate the index buffer
    for (unsigned int i = 0; i < aiMesh->mNumFaces ; ++i) 
	{
        const aiFace& face = aiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

bool Mesh::initMaterials(const aiScene* scene, const std::string& filename)
{
    // Extract the directory part from the file name
	std::string::size_type slashIndex = filename.find_last_of("/");
	std::string dir;

	if (slashIndex == std::string::npos) {
        dir = ".";
    }
    else if (slashIndex == 0) {
        dir = "/";
    }
    else {
        dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = scene->mMaterials[i];

        m_textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS) 
			{
				std::string p(Path.data);
				printf(p.c_str());
                
                if (p.substr(0, 2) == ".\\")                   
                    p = p.substr(2, p.size() - 2);
                               
				std::string fullPath = dir + "/" + p;
                    
                m_textures[i] = new Texture(fullPath.c_str());

                if (GLCheckError()) 
				{
                    printf("Error loading texture '%s'\n", fullPath.c_str());
                    delete m_textures[i];
                    m_textures[i] = NULL;
                    ret = false;
                }
				else 
				{
                    printf("Loaded texture '%s'\n", fullPath.c_str());
				}
            }
        }
    }
    return ret;
}

void Mesh::render()
{
    glBindVertexArray(m_vao);
    
    for (unsigned int i = 0 ; i < m_entries.size() ; i++) {
        const unsigned int materialIndex = m_entries[i].materialIndex;

        assert(materialIndex < m_textures.size());
        
        if (m_textures[materialIndex]) {
			m_textures[materialIndex]->bind();
        }

		glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_entries[i].numIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(unsigned int) * m_entries[i].baseIndex), 
                                 m_entries[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}

void Mesh::render(unsigned int numInstances, const glm::mat4* wvpMats, const glm::mat4* worldMats)
{        
    glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_MAT]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numInstances, wvpMats, GL_DYNAMIC_DRAW);

    
    for (unsigned int i = 0; i < m_entries.size(); ++i) 
	{
        const unsigned int materialIndex = m_entries[i].materialIndex;

        assert(materialIndex < m_textures.size());
        
        if (m_textures[materialIndex])
            m_textures[materialIndex]->bind();

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, 
                                          m_entries[i].numIndices, 
                                          GL_UNSIGNED_INT, 
                                          (void*)(sizeof(unsigned int) * m_entries[i].baseIndex), 
                                          numInstances,
                                          m_entries[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}

