#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

#include "VoxelWorld.h"

#include <iostream>
#include <stdio.h>

#include <vector>

//CRTP
template <typename Derived, typename Data>
class VoxelBlock
{
public:
	VoxelBlock()
	{
		std::cout << "const: " << std::endl;
		if (&VoxelBlock<Derived, Data>::onCreate != &Derived::onCreate)
			static_cast<Derived*>(this)->onCreate();
	};

	VoxelBlock(const VoxelBlock& copy)
	{
		std::cout << "copy" << std::endl;
	};

	~VoxelBlock()
	{
		std::cout << "dest" << std::endl;
	};

	void place()
	{
		if (&VoxelBlock<Derived, Data>::onPlace != &Derived::onPlace)
			static_cast<Derived*>(this)->onPlace();
	};

	void destroy()
	{
		if (&VoxelBlock<Derived, Data>::onDestroy != &Derived::onDestroy)
			static_cast<Derived*>(this)->onDestroy();
	};

	void blockUpdate()
	{
		if (&VoxelBlock<Derived, Data>::onBlockUpdate != &Derived::onBlockUpdate)
			static_cast<Derived*>(this)->onBlockUpdate();
	};

private:
	void onCreate() {};
	void onPlace() {};
	void onDestroy() {};
	void onBlockUpdate() {};
};

#endif //VOXEL_BLOCK_H_