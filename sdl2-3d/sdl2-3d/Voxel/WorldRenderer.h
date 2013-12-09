#ifndef WORLD_RENDERER_H_
#define WORLD_RENDERER_H_

class VoxelWorld;
class Camera;

class WorldRenderer
{
public:
	WorldRenderer();
	~WorldRenderer();
	WorldRenderer(const WorldRenderer& copy) = delete;

	void render(const VoxelWorld& world, const Camera& camera);

private:
	
};

#endif //WORLD_RENDERER_H_
