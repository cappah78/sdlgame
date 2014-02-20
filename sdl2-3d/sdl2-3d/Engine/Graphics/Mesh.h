#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <memory>

struct aiMesh;
struct aiScene;

class IShader;
class ITexture;
class IVertexBuffer;
class IConstantBuffer;
class IStateBuffer;

class IGraphicsProvider;

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
		: diffuseTexture(0)
		, normalTexture(0)
		, specularTexture(0)
		, opacityTexture(0)
		{};
		const ITexture* diffuseTexture;
		const ITexture* normalTexture;
		const ITexture* specularTexture;
		const ITexture* opacityTexture;
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
		ShaderAttributes(const IShader& shader, unsigned int materialUniformBufferBindingPoint = 1, const char* const materialUniformBufferName = "MaterialProperties",
			unsigned int positionLoc = 0, unsigned int texcoordLoc = 1, unsigned int normalLoc = 2,
			unsigned int colorLoc = 3, unsigned int tangentLoc = 4, unsigned int bitangentLoc = 5,
			unsigned int diffuseTextureBindLoc = 0, unsigned int normalTextureBindLoc = 1,
			unsigned int specularTextureBindLoc = 2, unsigned int opacityTextureBindLoc = 3)
			: shader(shader), materialUniformBufferBindingPoint(materialUniformBufferBindingPoint)
			, materialUniformBufferName(materialUniformBufferName)
			, positionLoc(positionLoc), texcoordLoc(texcoordLoc), normalLoc(normalLoc)
			, colorLoc(colorLoc), tangentLoc(tangentLoc), bitangentLoc(bitangentLoc)
			, diffuseTextureBindLoc(diffuseTextureBindLoc), normalTextureBindLoc(normalTextureBindLoc)
			, specularTextureBindLoc(specularTextureBindLoc), opacityTextureBindLoc(opacityTextureBindLoc)
		{};
		const IShader& shader;
		unsigned int materialUniformBufferBindingPoint;
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
	Mesh(const std::string& fileName, IGraphicsProvider& provider);
	~Mesh();

	/** Give data required to render the mesh*/
	void setShaderAttributes(std::shared_ptr<ShaderAttributes> shaderAttributes);
	void loadMesh(const std::string& filename, IGraphicsProvider& provider);

	//TODO: make private and write interface
	std::unique_ptr<IStateBuffer> m_stateBuffer;
	std::unique_ptr<IVertexBuffer> m_indiceBuffer;
	std::unique_ptr<IVertexBuffer> m_positionBuffer;
	std::unique_ptr<IVertexBuffer> m_normalBuffer;
	std::unique_ptr<IVertexBuffer> m_tangentBuffer;
	std::unique_ptr<IVertexBuffer> m_bitangentBuffer;
	std::unique_ptr<IVertexBuffer> m_texcoordBuffer;
	std::unique_ptr<IVertexBuffer> m_colorBuffer;
	std::unique_ptr<IConstantBuffer> m_matUniformBuffer;
	std::vector<MeshEntry> m_entries;
	std::vector<MeshMaterial> m_materials;
	std::vector<MeshMaterialProperties> m_matProperties;
	BufferFlags m_bufferFlags;
	std::shared_ptr<ShaderAttributes> m_shaderAttributes;

	void render();

private:

	void initVertexBuffers(const aiScene* scene);
	void initMaterials(const aiScene* scene, const std::string& filename, IGraphicsProvider& provider);
};