#include "mesh.h"
#include "Texture.h"

#include <assert.h>
#include <gl\glew.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

static const aiVector3D ZERO_VEC(0.0f, 0.0f, 0.0f);

Mesh::Mesh() 
{

}

Mesh::Mesh(const std::string& fileName)
{
	loadMesh(fileName);
}

Mesh::~Mesh()
{

}

void Mesh::loadMesh(const std::string& filename)
{
	const aiScene* scene = aiImportFile(filename.c_str(), aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_GenSmoothNormals | aiPostProcessSteps::aiProcess_FlipUVs);

	if (!scene)
		printf("Error parsing '%s' : %s\n", filename.c_str(), aiGetErrorString());

	initVertexBuffers(scene);
	if (!initMaterials(scene, filename))
		return;
}

void Mesh::initVertexBuffers(const aiScene* scene)
{
	glBindVertexArray(0);

	m_entries.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<glm::vec2>> texCoords;
	std::vector<std::vector<glm::vec4>> colors;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> indices;

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	assert(scene->mMeshes[0]->HasFaces());

	bool hasNormals = false;
	bool hasTangents = false;
	unsigned int numTexCoordChannels = 0;
	unsigned int numVertexColorChannels = 0;

	// find all the used channels
	for (unsigned int i = 0; i < m_entries.size(); ++i)
	{
		if (scene->mMeshes[i]->HasNormals())
			hasNormals = true;
		if (scene->mMeshes[i]->HasTangentsAndBitangents())
			hasTangents = true;

		unsigned int texCoordCount = scene->mMeshes[i]->GetNumUVChannels();
		if (texCoordCount > numTexCoordChannels)
			numTexCoordChannels = texCoordCount;
		unsigned int vertexColorCount = scene->mMeshes[i]->GetNumUVChannels();
		if (vertexColorCount > numVertexColorChannels)
			numVertexColorChannels = vertexColorCount;
	}

	// count the number of vertices and indices, save the base vertex of every sub mesh
	for (unsigned int i = 0; i < m_entries.size(); i++)
	{
		m_entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
		m_entries[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
		m_entries[i].baseVertex = numVertices;
		m_entries[i].baseIndex = numIndices;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += m_entries[i].numIndices;
	}

	// reserve all the things
	indices.reserve(numIndices);
	positions.reserve(numVertices);
	if (hasNormals)
		normals.reserve(numVertices);
	if (hasTangents)
	{
		tangents.reserve(numVertices);
		bitangents.reserve(numVertices);
	}
	texCoords.reserve(numTexCoordChannels);
	for (unsigned int i = 0; i < numTexCoordChannels; ++i)
		texCoords.at(i).reserve(numVertices);
	colors.reserve(numVertexColorChannels);
	for (unsigned int i = 0; i < numVertexColorChannels; ++i)
		colors.at(i).reserve(numVertices);

	/* FIXME! 
	* TODO: the max number of attributes are used, so if theres a submesh 
	* that doesnt have the same amount of attributes, things go wrong.
	*/

	// for every mesh in the scene
	for (unsigned int i = 0; i < m_entries.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		const unsigned int numVertices = mesh->mNumVertices;
		const unsigned int numFaces = mesh->mNumFaces;

		for (unsigned int j = 0; j < numFaces; ++j)
		{
			const aiFace& face = mesh->mFaces[j];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}


		for (unsigned int j = 0; j < numVertices; ++j)
		{
			const aiVector3D& vec = mesh->mVertices[j];
			positions.push_back(glm::vec3(vec.x, vec.y, vec.z));
		}

		if (hasNormals)
		{
			for (unsigned int j = 0; j < numVertices; ++j)
			{
				const aiVector3D& vec = mesh->mNormals[j];
				normals.push_back(glm::vec3(vec.x, vec.y, vec.z));
			}
		}
		if (hasTangents)
		{
			for (unsigned int j = 0; j < numVertices; ++j)
			{
				const aiVector3D& vec = mesh->mTangents[j];
				tangents.push_back(glm::vec3(vec.x, vec.y, vec.z));
			}
			for (unsigned int j = 0; j < numVertices; ++j)
			{
				const aiVector3D& vec = mesh->mBitangents[j];
				bitangents.push_back(glm::vec3(vec.x, vec.y, vec.z));
			}
		}
		for (unsigned int channel = 0; channel < numTexCoordChannels; ++channel)
		{
			std::vector<glm::vec2>& list = texCoords.at(i);
			for (unsigned int j = 0; j < numVertices; ++j)
			{
				const aiVector3D& vec = mesh->mTextureCoords[i][j];
				list.push_back(glm::vec2(vec.x, vec.y));
			}
		}
		for (unsigned int channel = 0; channel < numVertexColorChannels; ++channel)
		{
			std::vector<glm::vec4>& list = colors.at(i);
			for (unsigned int j = 0; j < numVertices; ++j)
			{
				const aiColor4D& col = mesh->mColors[i][j];
				list.push_back(glm::vec4(col.r, col.g, col.b, col.a));
			}
		}
	}

	m_indiceBuffer.reset(new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER));
	m_indiceBuffer->upload(&indices[0], sizeof(indices[0]) * indices.size());

	m_positionBuffer.reset(new VertexBuffer());
	m_positionBuffer->upload(&positions[0], sizeof(positions[0]) * positions.size());

	if (hasTangents)
	{
		m_tangentBuffer.reset(new VertexBuffer());
		m_tangentBuffer->upload(&tangents[0], sizeof(tangents[0]) * tangents.size());

		m_bitangentBuffer.reset(new VertexBuffer());
		m_bitangentBuffer->upload(&bitangents[0], sizeof(bitangents[0]) * bitangents.size());
	}
	m_texcoordBuffers.resize(numTexCoordChannels);
	for (unsigned int i = 0; i < numTexCoordChannels; ++i)
	{
		m_texcoordBuffers.at(i).reset(new VertexBuffer());
		m_texcoordBuffers.at(i)->upload(&texCoords[i][0], sizeof(texCoords[i][0]) * texCoords[i].size());
	}
	m_colorBuffers.resize(numVertexColorChannels);
	for (unsigned int i = 0; i < numVertexColorChannels; ++i)
	{
		m_colorBuffers.push_back(std::unique_ptr<VertexBuffer>(new VertexBuffer()));
		m_colorBuffers.at(i)->upload(&colors[i][0], sizeof(colors[i][0]) * colors[i].size());
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
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];

		m_textures[i] = NULL;


		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string p(path.data);
				printf(p.c_str());

				if (p.substr(0, 2) == ".\\")
					p = p.substr(2, p.size() - 2);

				std::string fullPath = dir + "/" + p;

				m_textures[i] = new Texture(fullPath.c_str());
				if (!m_textures[i]->isLoaded())
					delete m_textures[i];
					m_textures[i] = NULL;
					ret = false;
				}
			}
		}
	return ret;
}

/*
void Mesh::render()
{
	glBindVertexArray(m_vao);

	for (unsigned int i = 0; i < m_entries.size(); i++) {
		const unsigned int materialIndex = m_entries[i].materialIndex;

		assert(materialIndex < m_textures.size());

		if (m_textures[materialIndex]) {
			m_textures[materialIndex]->bind();
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_entries[i].numIndices,
			GL_UNSIGNED_INT,
			(void*) (sizeof(unsigned int) * m_entries[i].baseIndex),
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
			(void*) (sizeof(unsigned int) * m_entries[i].baseIndex),
			numInstances,
			m_entries[i].baseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}
*/
