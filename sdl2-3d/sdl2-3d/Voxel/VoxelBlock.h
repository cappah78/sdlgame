#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

struct VoxelBlockProperties	// 9 bytes;
{
	unsigned topTexture : 10;
	unsigned bottomTexture : 10;
	unsigned leftTexture : 10;
	unsigned rightTexture : 10;
	unsigned frontTexture : 10;
	unsigned backTexture : 10;
	unsigned isTranspant : 1;
	unsigned isSolid : 1;
	unsigned strength : 10;
};

//CRTP
template <class Implementation, typename Data>
class VoxelBlock
{
public:
	VoxelBlock()
	{

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