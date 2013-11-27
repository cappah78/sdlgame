#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

#include "LuaHelper.h"

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
		LuaHelper::createBlockType(typeid(Implementation).name());
	};

	VoxelBlock(const VoxelBlock& copy)
	{
		//std::cout << "copy" << std::endl;
	};

	~VoxelBlock()
	{
		//std::cout << "dest" << std::endl;
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