#ifndef MESH_BLOCK_H_
#define MESH_BLOCK_H_

#include "VoxelBlock.h"

class Mesh;

class MeshBlock : public VoxelBlock<MeshBlock>
{
public:
	//TODO: all the things

	static void onCreate(int x, int y, int z);
	static void onPlace(int x, int y, int z);
	static void onDestroy(int x, int y, int z);
	static void onBlockUpdate(int x, int y, int z);
private:
	Mesh* mesh;
};

#endif //MESH_BLOCK_H_