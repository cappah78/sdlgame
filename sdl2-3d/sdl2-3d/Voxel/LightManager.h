#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_

#include "VoxelWorld.h"
#include "../Engine/Graphics/Camera.h"

//TODO: everything.

class LightManager
{
public:
	static const int SKY_LIGHT_LEVEL = 255;

	LightManager();
	~LightManager();
	LightManager(const LightManager& copy) = delete;
private:
	void applySkyLight(VoxelWorld& world, const Camera& camera);
};

#endif //LIGHT_MANAGER_H_
