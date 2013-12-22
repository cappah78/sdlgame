#include "BlockRenderProperties.h"

#include "PropertyManager.h"
#include "TextureManager.h"

BlockRenderProperties::DefaultRenderProperties::DefaultRenderProperties(LuaTableData& data)
{
	/*
	luabridge::LuaRef block = data.ref;
	const char* topTexCStr = texture["top"];				//read the texture paths
	const char* bottomTexCStr = texture["bottom"];
	const char* leftTexCStr = texture["left"];
	const char* rightTexCStr = texture["right"];
	const char* frontTexCStr = texture["front"];
	const char* backTexCStr = texture["back"];


	bool isTransparent;
	if (block["transparent"].isNil())
		isTransparent = false;	//default transparency is false
	else
		isTransparent = block["transparent"];

	bool isSolid;
	if (block["solid"].isNil())
		isSolid = false;	//default solidity is true
	else
		isSolid = block["solid"];

	assert(!block["strength"].isNil() && "Block strength not declared");
	assert(topTexCStr != NULL && "Top face texture not declared");
	assert(bottomTexCStr != NULL && "Bottom face texture not declared");
	assert(leftTexCStr != NULL && "Left face texture not declared");
	assert(rightTexCStr != NULL && "Right face texture not declared");
	assert(frontTexCStr != NULL && "Front face texture not declared");
	assert(backTexCStr != NULL && "Back face texture not declared");

	short blockStrength = block["strength"];

	TextureID topID = m_textureManager.getTextureID(topTexCStr);
	TextureID bottomID = m_textureManager.getTextureID(bottomTexCStr);
	TextureID leftID = m_textureManager.getTextureID(leftTexCStr);
	TextureID rightID = m_textureManager.getTextureID(rightTexCStr);
	TextureID frontID = m_textureManager.getTextureID(frontTexCStr);
	TextureID backID = m_textureManager.getTextureID(backTexCStr);
	*/
}