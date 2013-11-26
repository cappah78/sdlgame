#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

#include "VoxelWorld.h"

#include <iostream>
#include <string>
#include <stdio.h>

#include <vector>

#include "../Game.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include "../Engine/Utils/CheckLuaError.h"

typedef unsigned short BlockID;
typedef unsigned short BlockStrength;

struct VoxelBlockProperties
{
	BlockStrength blockStrength;
	bool isTransparant;
	bool isAnimated;
};

//CRTP
template <class Implementation, typename Data>
class VoxelBlock
{
public:
	VoxelBlock()
	{
		//typeid(T) works even with RTTI disabled
		std::string implmentationName = std::string(typeid(Implementation).name());
		std::size_t found = implmentationName.find("class ");
		if (found == std::string::npos) //null implementation
		{
			
		}
		else
		{
			std::string name = implmentationName.substr(6); //substr to remove 'class '

			std::string scriptPath("Scripts/");
			scriptPath.append(name);
			scriptPath.append(".lua");

			std::string initLuaBlock("Blocks.");
			initLuaBlock.append(name);
			initLuaBlock.append(" = {}");
			luaL_dostring(Game::L, initLuaBlock.c_str());

			checkLuaError(Game::L, luaL_dofile(Game::L, scriptPath.c_str()));

			luabridge::LuaRef ref = luabridge::getGlobal(Game::L, "Blocks");
			if (ref.isNil())
				return;
			luabridge::LuaRef ref2 = ref[name];
			if (ref2.isNil())
				return;
			luabridge::LuaRef ref3 = ref2["test"];
			if (ref3.isNil())
				return;
		
			try {
				ref3();
			}
			catch (luabridge::LuaException const& e) {
				std::cout << e.what() << std::endl;
			}
		}


	};

	VoxelBlock(const VoxelBlock& copy)
	{
		std::cout << "copy" << std::endl;
	};

	~VoxelBlock()
	{
		std::cout << "dest" << std::endl;
	};

	VoxelBlockProperties registerType()
	{
		return static_cast<Implementation*>(this)->onRegisterType();
	};

	void place()
	{
		if (&VoxelBlock<Implementation, Data>::onPlace != &Implementation::onPlace)
			static_cast<Implementation*>(this)->onPlace();
	};

	void destroy()
	{
		if (&VoxelBlock<Implementation, Data>::onDestroy != &Implementation::onDestroy)
			static_cast<Implementation*>(this)->onDestroy();
	};

	void blockUpdate()
	{
		if (&VoxelBlock<Implementation, Data>::onBlockUpdate != &Implementation::onBlockUpdate)
			static_cast<Implementation*>(this)->onBlockUpdate();
	};

private:
	void onCreate() {};
	void onPlace() {};
	void onDestroy() {};
	void onBlockUpdate() {};
};

#endif //VOXEL_BLOCK_H_