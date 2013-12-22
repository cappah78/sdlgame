#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

typedef unsigned short BlockID;
typedef unsigned short BlockDataSize;

class VoxelBlock
{
public:
	enum RenderType {
		DEFAULT_BLOCK = 0, MESH_BLOCK = 1
	};
private:
	VoxelBlock();
};

#endif //VOXEL_BLOCK_H_
