#include "DefaultBlockRenderProperties.h"

#include "PropertyManager.h"
#include "TextureManager.h"

DefaultBlockRenderProperties::DefaultBlockRenderProperties(LuaTableData& data, TextureManager& textureManager)
{
	luabridge::LuaRef block = data.ref;
	luabridge::LuaRef texture = block["texture"];
	const char* topTexCStr = texture["top"];				//read the texture paths
	const char* bottomTexCStr = texture["bottom"];
	const char* leftTexCStr = texture["left"];
	const char* rightTexCStr = texture["right"];
	const char* frontTexCStr = texture["front"];
	const char* backTexCStr = texture["back"];

	isTransparent = block["transparent"].isNil() ? block["transparent"] : false;
	isAnimated = block["animated"].isNil() ? block["animated"] : false;

	assert(!block["strength"].isNil() && "Block strength not declared");
	assert(topTexCStr != NULL && "Top face texture not declared");
	assert(bottomTexCStr != NULL && "Bottom face texture not declared");
	assert(leftTexCStr != NULL && "Left face texture not declared");
	assert(rightTexCStr != NULL && "Right face texture not declared");
	assert(frontTexCStr != NULL && "Front face texture not declared");
	assert(backTexCStr != NULL && "Back face texture not declared");

	topTexture = textureManager.getTextureID(topTexCStr);
	bottomTexture = textureManager.getTextureID(bottomTexCStr);
	leftTexture = textureManager.getTextureID(leftTexCStr);
	rightTexture = textureManager.getTextureID(rightTexCStr);
	frontTexture = textureManager.getTextureID(frontTexCStr);
	backTexture = textureManager.getTextureID(backTexCStr);
}