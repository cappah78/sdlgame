#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

typedef unsigned short BlockID;
typedef unsigned short TextureID;

class VoxelWorld;

//CRTP
template <class Implementation>
class VoxelBlock
{
public:

	VoxelBlock(BlockID blockID)
		: m_blockID(blockID)
	{};

	~VoxelBlock()
	{

	};

	static void create(VoxelWorld& world, int x, int y, int z)
	{
		if (&VoxelBlock<Implementation>::onCreate != &Implementation::onCreate)
			static_cast<Implementation*>(this)->onCreate(world, x, y, z);
	}

	static void place(VoxelWorld& world, int x, int y, int z)
	{
		if (&VoxelBlock<Implementation>::onPlace != &Implementation::onPlace)
			static_cast<Implementation*>(this)->onPlace(world, x, y, z);
	}

	static void destroy(VoxelWorld& world, int x, int y, int z)
	{
		if (&VoxelBlock<Implementation>::onDestroy != &Implementation::onDestroy)
			static_cast<Implementation*>(this)->onDestroy(world, x, y, z);
	};

	static void blockUpdate(VoxelWorld& world, int x, int y, int z)
	{
		if (&VoxelBlock<Implementation>::onBlockUpdate != &Implementation::onBlockUpdate)
			static_cast<Implementation*>(this)->onBlockUpdate(world, x, y, z);
	};

private:
	void onCreate(VoxelWorld& world, int x, int y, int z) {};
	void onPlace(VoxelWorld& world, int x, int y, int z) {};
	void onDestroy(VoxelWorld& world, int x, int y, int z) {};
	void onBlockUpdate(VoxelWorld& world, int x, int y, int z) {};
};

#endif //VOXEL_BLOCK_H_