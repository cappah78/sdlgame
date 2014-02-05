#include "Mesh.h"
#include "Texture.h"

#include <assert.h>
#include <gl\glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

static const aiVector3D ZERO_VEC(0.0f, 0.0f, 0.0f);

Mesh::Mesh() 
{

}

Mesh::Mesh(const std::string& fileName, TextureManager& textureManager)
{
	loadMesh(fileName, textureManager);
}

Mesh::~Mesh()
{

}

void Mesh::loadMesh(const std::string& filename, TextureManager& textureManager)
{
	const aiScene* scene = aiImportFile(filename.c_str(), aiPostProcessSteps::aiProcess_Triangulate 
		| aiPostProcessSteps::aiProcess_GenSmoothNormals 
		| aiPostProcessSteps::aiProcess_FlipUVs 
		| aiPostProcessSteps::aiProcess_CalcTangentSpace 
		| aiPostProcessSteps::aiProcess_JoinIdenticalVertices 
		| aiPostProcessSteps::aiProcess_ImproveCacheLocality
		| aiPostProcessSteps::aiProcess_OptimizeMeshes);

	if (!scene)
		printf("Error parsing '%s' : %s\n", filename.c_str(), aiGetErrorString());

	initVertexBuffers(scene);
	initMaterials(scene, filename, textureManager);
}

void Mesh::initVertexBuffers(const aiScene* scene)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_matUbo);

	printf("nummeshes: %i nummaterials: %i \n", scene->mNumMeshes, scene->mNumMaterials);

	m_entries.reserve(scene->mNumMeshes);
	m_materials.reserve(scene->mNumMaterials);
	m_matProperties.reserve(scene->mNumMaterials);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> indices;

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	// find all the used channels
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		if (scene->mMeshes[i]->HasFaces())
			m_bufferFlags.hasIndiceBuffer = true;
		if (scene->mMeshes[i]->HasPositions())
			m_bufferFlags.hasPositionBuffer = true;
		if (scene->mMeshes[i]->HasNormals())
			m_bufferFlags.hasNormalBuffer = true;
		if (scene->mMeshes[i]->HasTangentsAndBitangents())
			m_bufferFlags.hasTangentBitangentBuffer = true;
		if (scene->mMeshes[i]->HasTextureCoords(0))
			m_bufferFlags.hasTexcoordBuffer = true;
		if (scene->mMeshes[i]->HasVertexColors(0))
			m_bufferFlags.hasColorBuffer = true;
	}

	// indice buffer required?
	assert(m_bufferFlags.hasIndiceBuffer);

	// count the number of vertices and indices, save the base vertex of every sub mesh
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		MeshEntry entry;
		entry.materialIndex = scene->mMeshes[i]->mMaterialIndex;
		entry.numIndices = scene->mMeshes[i]->mNumFaces * 3;
		entry.baseVertex = numVertices;
		entry.baseIndex = numIndices;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += entry.numIndices;
		m_entries.push_back(entry);
	}

	// reserve all the things
	if (m_bufferFlags.hasIndiceBuffer)
		indices.reserve(numIndices);
	if (m_bufferFlags.hasPositionBuffer)
	{
		positions.reserve(numVertices);
	}
	if (m_bufferFlags.hasNormalBuffer)
		normals.reserve(numVertices);
	if (m_bufferFlags.hasTangentBitangentBuffer)
	{
		tangents.reserve(numVertices);
		bitangents.reserve(numVertices);
	}
	if (m_bufferFlags.hasTexcoordBuffer)
		texCoords.reserve(numVertices);
	if (m_bufferFlags.hasColorBuffer)
		colors.reserve(numVertices);

	/* FIXME! 
	* TODO: the max number of attributes are used, so if theres a submesh 
	* that doesnt have the same amount of attributes, things go wrong.
	*/

	// for every mesh in the scene
	for (unsigned int i = 0; i < m_entries.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		const unsigned int numVerts = mesh->mNumVertices;
		const unsigned int numFaces = mesh->mNumFaces;

		if (m_bufferFlags.hasIndiceBuffer)
		{
			assert(mesh->HasFaces());
			for (unsigned int j = 0; j < numFaces; ++j)
			{
				const aiFace& face = mesh->mFaces[j];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}
		}
		if (m_bufferFlags.hasPositionBuffer)
		{
			assert(mesh->HasPositions());
			positions.insert(positions.end(), (glm::vec3*) mesh->mVertices, (glm::vec3*) mesh->mVertices + numVerts);
		}
		if (m_bufferFlags.hasNormalBuffer)
		{
			assert(mesh->HasNormals());
			normals.insert(normals.end(), (glm::vec3*) mesh->mNormals, (glm::vec3*) mesh->mNormals + numVerts);
		}
		if (m_bufferFlags.hasTangentBitangentBuffer)
		{
			assert(mesh->HasTangentsAndBitangents());
			tangents.insert(tangents.end(), (glm::vec3*) mesh->mTangents, (glm::vec3*) mesh->mTangents + numVerts);
			bitangents.insert(bitangents.end(), (glm::vec3*) mesh->mBitangents, (glm::vec3*) mesh->mBitangents + numVerts);
		}
		if (m_bufferFlags.hasTexcoordBuffer)
		{
			assert(mesh->HasTextureCoords(0));
			for (unsigned int j = 0; j < numVerts; ++j)
			{
				const aiVector3D& vec = mesh->mTextureCoords[0][j];
				texCoords.push_back(glm::vec2(vec.x, vec.y));
			}
		}
		if (m_bufferFlags.hasColorBuffer)
		{
			assert(mesh->HasVertexColors(0));
			for (unsigned int j = 0; j < numVerts; ++j)
			{
				const aiColor4D& vec = mesh->mColors[0][j];
				colors.push_back(glm::vec4(vec.r, vec.g, vec.b, vec.a));
			}
		}
	}

	if (m_bufferFlags.hasIndiceBuffer)
	{
		m_indiceBuffer.reset(new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER));
		m_indiceBuffer->upload(&indices[0], sizeof(indices[0]) * indices.size());
	}
	if (m_bufferFlags.hasPositionBuffer)
	{
		m_positionBuffer.reset(new VertexBuffer());
		m_positionBuffer->upload(&positions[0], sizeof(positions[0]) * positions.size());
	}
	if (m_bufferFlags.hasNormalBuffer)
	{
		m_normalBuffer.reset(new VertexBuffer());
		m_normalBuffer->upload(&normals[0], sizeof(normals[0]) * normals.size());
	}
	if (m_bufferFlags.hasTangentBitangentBuffer)
	{
		m_tangentBuffer.reset(new VertexBuffer());
		m_tangentBuffer->upload(&tangents[0], sizeof(tangents[0]) * tangents.size());

		m_bitangentBuffer.reset(new VertexBuffer());
		m_bitangentBuffer->upload(&bitangents[0], sizeof(bitangents[0]) * bitangents.size());
	}
	if (m_bufferFlags.hasTexcoordBuffer)
	{
		m_texcoordBuffer.reset(new VertexBuffer());
		m_texcoordBuffer->upload(&texCoords[0], sizeof(texCoords[0]) * texCoords.size());
	}
	if (m_bufferFlags.hasColorBuffer)
	{
		m_colorBuffer.reset(new VertexBuffer());
		m_colorBuffer->upload(&colors[0], sizeof(colors[0]) * colors.size());
	}
}

void Mesh::initMaterials(const aiScene* scene, const std::string& filename, TextureManager& textureManager)
{
	// Extract the directory part from the file name
	std::string::size_type slashIndex = filename.find_last_of("/");
	std::string dir;
	if (slashIndex == std::string::npos)
		dir = ".";
	else if (slashIndex == 0)
		dir = "/";
	else
		dir = filename.substr(0, slashIndex);

	// Initialize the materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		MeshMaterial meshMaterial;

		int numDiffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
		int numNormal = material->GetTextureCount(aiTextureType_HEIGHT);
		int numSpecular = material->GetTextureCount(aiTextureType_SPECULAR);
		int numOpacity = material->GetTextureCount(aiTextureType_OPACITY);

		MeshMaterialProperties matProperties;

		aiColor3D difCol(1.0f, 1.0f, 1.0f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, difCol);
		matProperties.diffuseColor = *((glm::vec3*) &difCol);
		//printf("difcol: %f, %f, %f \n", difCol.r, difCol.g, difCol.b);

		aiColor3D specCol(1.0f, 1.0f, 1.0f);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specCol);
		matProperties.specularColor = *((glm::vec3*) &specCol);
		//printf("specCol: %f, %f, %f \n", specCol.r, specCol.g, specCol.b);

		aiColor3D emisCol(1.0f, 1.0f, 1.0f);
		material->Get(AI_MATKEY_COLOR_EMISSIVE, emisCol);
		matProperties.specularColor = *((glm::vec3*) &emisCol);
		//printf("emisCol: %f, %f, %f \n", emisCol.r, emisCol.g, emisCol.b);

		float transp(1.0f);
		material->Get(AI_MATKEY_OPACITY, transp);
		matProperties.materialAlpha = transp;
		//printf("transp %f \n", transp);

		float specExp(0.0f);	//whats default?
		material->Get(AI_MATKEY_OPACITY, specExp);
		matProperties.materialSpecExp = specExp;
		//printf("specExp %f \n", specExp);

		m_matProperties.push_back(matProperties);

		aiString path;

		if (numDiffuse > 0 && material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			std::string p(path.data);
			if (p.substr(0, 2) == ".\\")
				p = p.substr(2, p.size() - 2);
			std::string fullPath = dir + "/" + p;
			meshMaterial.diffuseTexture = textureManager.getTextureID(fullPath);
		}
		if (numNormal > 0 && material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
		{
			std::string p(path.data);
			if (p.substr(0, 2) == ".\\")
				p = p.substr(2, p.size() - 2);
			std::string fullPath = dir + "/" + p;
			meshMaterial.normalTexture = textureManager.getTextureID(fullPath);
		}
		if (numSpecular > 0 && material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
		{
			std::string p(path.data);
			if (p.substr(0, 2) == ".\\")
				p = p.substr(2, p.size() - 2);
			std::string fullPath = dir + "/" + p;
			meshMaterial.specularTexture = textureManager.getTextureID(fullPath);
		}
		if (numOpacity > 0 && material->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
		{
			std::string p(path.data);
			if (p.substr(0, 2) == ".\\")
				p = p.substr(2, p.size() - 2);
			std::string fullPath = dir + "/" + p;
			meshMaterial.opacityTexture = textureManager.getTextureID(fullPath);
		}
		m_materials.push_back(meshMaterial);
		//printf("Materials: %i %i %i %i \n", meshMaterial.diffuseTexture, meshMaterial.normalTexture, meshMaterial.specularTexture, meshMaterial.opacityTexture);
	}
}

void Mesh::setShaderAttributes(std::shared_ptr<ShaderAttributes> shaderAttributes)
{
	m_shaderAttributes = shaderAttributes;
	glBindVertexArray(m_vao);
	if (m_bufferFlags.hasPositionBuffer && shaderAttributes->positionLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC)
		m_positionBuffer->setAttribPointer(shaderAttributes->positionLoc, GL_FLOAT, 3);
	if (m_bufferFlags.hasNormalBuffer && shaderAttributes->normalLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC)
		m_normalBuffer->setAttribPointer(shaderAttributes->normalLoc, GL_FLOAT, 3);
	if (m_bufferFlags.hasTexcoordBuffer && shaderAttributes->texcoordLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC)
		m_texcoordBuffer->setAttribPointer(shaderAttributes->texcoordLoc, GL_FLOAT, 2);
	if (m_bufferFlags.hasColorBuffer && shaderAttributes->colorLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC)
		m_colorBuffer->setAttribPointer(shaderAttributes->colorLoc, GL_FLOAT, 4);
	if (m_bufferFlags.hasTangentBitangentBuffer
		&& shaderAttributes->tangentLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC
		&& shaderAttributes->bitangentLoc != ShaderAttributes::UNUSED_VERTEX_ATTRIBUTE_LOC)
	{
		m_tangentBuffer->setAttribPointer(shaderAttributes->tangentLoc, GL_FLOAT, 3);
		m_bitangentBuffer->setAttribPointer(shaderAttributes->bitangentLoc, GL_FLOAT, 3);
	}

	m_matUboIndex = glGetUniformBlockIndex(shaderAttributes->shaderID, shaderAttributes->materialUniformBufferName);
	m_matUboBindingPoint = shaderAttributes->materialUniformBufferBindingPoint;

	glBindBuffer(GL_UNIFORM_BUFFER, m_matUbo);
	glUniformBlockBinding(shaderAttributes->shaderID, m_matUboIndex, m_matUboBindingPoint);
	printf("block index: %i \n", m_matUboIndex);
}


void Mesh::render()
{
	glBindVertexArray(m_vao);

	for (unsigned int i = 0; i < m_entries.size(); i++) {
		const MeshEntry& entry = m_entries.at(i);
		const unsigned int materialIndex = entry.materialIndex;
		const MeshMaterial& material = m_materials.at(materialIndex);
		const MeshMaterialProperties& matProperties = m_matProperties.at(materialIndex);

		glBindBuffer(GL_UNIFORM_BUFFER, m_matUbo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(MeshMaterialProperties), &matProperties, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_matUboBindingPoint, m_matUbo);

		if (material.diffuseTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->diffuseTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, material.diffuseTexture);
		}
		if (material.normalTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->normalTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, material.normalTexture);
		}
		if (material.specularTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->specularTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, material.specularTexture);
		}
		if (material.opacityTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->opacityTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, material.opacityTexture);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			entry.numIndices,
			GL_UNSIGNED_INT,
			(void*) (sizeof(unsigned int) * entry.baseIndex),
			entry.baseVertex);

		if (material.diffuseTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->diffuseTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (material.normalTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->normalTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (material.specularTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->specularTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (material.opacityTexture != INVALID_TEXTURE_ID)
		{
			glActiveTexture(GL_TEXTURE0 + m_shaderAttributes->opacityTextureBindLoc);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindVertexArray(0);
}
/*
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
