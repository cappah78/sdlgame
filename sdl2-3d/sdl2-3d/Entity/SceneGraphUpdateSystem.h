#pragma once

#include "SceneGraphComponent.h"
#include "KinematicsComponent.h"
#include "../Engine/Entity/ComponentUpdateSystem.h"

class SceneGraphUpdateSystem : public ComponentUpdateSystem<SceneGraphComponent, KinematicsComponent>
{
public:
	SceneGraphUpdateSystem() {};
	virtual ~SceneGraphUpdateSystem() {};
	virtual void updateComponents(std::vector<SceneGraphComponent>& update, const std::vector<KinematicsComponent>& with);

private:
};