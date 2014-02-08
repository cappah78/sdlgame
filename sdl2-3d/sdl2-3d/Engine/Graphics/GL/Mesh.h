#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <memory>
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "../TextureManager.h"

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
	struct MeshMaterial
	{
		MeshMaterial()
		: diffuseTexture(INVALID_TEXTURE_ID)
		, normalTexture(INVALID_TEXTURE_ID)
		, specularTexture(INVALID_TEXTURE_ID)
		, opacityTexture(INVALID_TEXTURE_ID)
		{};
		GLuint diffuseTexture;
		GLuint normalTexture;
		GLuint specularTexture;
		GLuint opacityTexture;
	};

	struct MeshMaterialProperties
	{
		glm::vec3 diffuseColor;
		float materialAlpha;
		glm::vec3 specularColor;
		float materialSpecExp;
		glm::vec3 materialEmissive;
		float padding;
	};

	struct BufferFlags
	{
		BufferFlags()
		: hasIndiceBuffer(false)
		, hasPositionBuffer(false)
		, hasTexcoordBuffer(false)
		, hasNormalBuffer(false)
		, hasColorBuffer(false)
		, hasTangentBitangentBuffer(false)
		{};
		unsigned hasIndiceBuffer : 1;
		unsigned hasPositionBuffer : 1;
		unsigned hasTexcoordBuffer : 1;
		unsigned hasNormalBuffer : 1;
		unsigned hasColorBuffer : 1;
		unsigned hasTangentBitangentBuffer : 1;
	};
	struct ShaderAttributes
	{
		static const unsigned int UNUSED_VERTEX_ATTRIBUTE_LOC = 0xFFFFFFFF;
		ShaderAttributes(GLuint shaderID, GLuint materialUniformBufferBindingPoint = 1, const char* const materialUniformBufferName = "MaterialProperties",
			unsigned int positionLoc = 0, unsigned int texcoordLoc = 1, unsigned int normalLoc = 2,
			unsigned int colorLoc = 3, unsigned int tangentLoc = 4, unsigned int bitangentLoc = 5,
			unsigned int diffuseTextureBindLoc = 0, unsigned int normalTextureBindLoc = 1,
			unsigned int specularTextureBindLoc = 2, unsigned int opacityTextureBindLoc = 3)
			: shaderID(shaderID), materialUniformBufferBindingPoint(materialUniformBufferBindingPoint)
			, materialUniformBufferName(materialUniformBufferName)
			, positionLoc(positionLoc), texcoordLoc(texcoordLoc), normalLoc(normalLoc)
			, colorLoc(colorLoc), tangentLoc(tangentLoc), bitangentLoc(bitangentLoc)
			, diffuseTextureBindLoc(diffuseTextureBindLoc), normalTextureBindLoc(normalTextureBindLoc)
			, specularTextureBindLoc(specularTextureBindLoc), opacityTextureBindLoc(opacityTextureBindLoc)
		{};
		GLuint shaderID;
		GLuint materialUniformBufferBindingPoint;
		const char* const materialUniformBufferName;
		unsigned int positionLoc;
		unsigned int texcoordLoc;
		unsigned int normalLoc;
		unsigned int colorLoc;
		unsigned int tangentLoc;
		unsigned int bitangentLoc;
		unsigned int diffuseTextureBindLoc;
		unsigned int normalTextureBindLoc;
		unsigned int specularTextureBindLoc;
		unsigned int opacityTextureBindLoc;
	};

	Mesh();
	Mesh(const std::string& fileName, TextureManager& textureManager);

	~Mesh();
	
	/** Give data required to render the mesh*/
	void setShaderAttributes(std::shared_ptr<ShaderAttributes> shaderAttributes);
	void loadMesh(const std::string& filename, TextureManager& textureManager);

	//TODO: make private and write interface
	GLuint m_vao;
	std::unique_ptr<VertexBuffer> m_indiceBuffer;
	std::unique_ptr<VertexBuffer> m_positionBuffer;
	std::unique_ptr<VertexBuffer> m_normalBuffer;
	std::unique_ptr<VertexBuffer> m_tangentBuffer;
	std::unique_ptr<VertexBuffer> m_bitangentBuffer;
	std::unique_ptr<VertexBuffer> m_texcoordBuffer;
	std::unique_ptr<VertexBuffer> m_colorBuffer;
	std::unique_ptr<ConstantBuffer> m_matUniformBuffer;
	std::vector<MeshEntry> m_entries;
	std::vector<MeshMaterial> m_materials;
	std::vector<MeshMaterialProperties> m_matProperties;
	GLuint m_matUbo;
	GLuint m_matUboBindingPoint;
	GLuint m_matUboIndex;
	BufferFlags m_bufferFlags;
	std::shared_ptr<ShaderAttributes> m_shaderAttributes;

	void render();
	//void render(unsigned int numInstances, const glm::mat4* wvpMats, const glm::mat4* worldMats);

private:

	void initVertexBuffers(const aiScene* scene);
	void initMaterials(const aiScene* scene, const std::string& filename, TextureManager& textureManager);
};