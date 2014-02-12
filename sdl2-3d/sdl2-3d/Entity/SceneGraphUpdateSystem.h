#pragma once

#include "../Engine/Entity/ComponentUpdateSystem.h"
#include "SceneGraphComponent.h"
#include "KinematicsComponent.h"

class SceneGraphUpdateSystem : public ComponentUpdateSystem<SceneGraphComponent, KinematicsComponent>
{
public:
	SceneGraphUpdateSystem() {};
	virtual ~SceneGraphUpdateSystem() {};
	virtual void updateComponents(std::vector<SceneGraphComponent>& update, const std::vector<KinematicsComponent>& with);

private:
};