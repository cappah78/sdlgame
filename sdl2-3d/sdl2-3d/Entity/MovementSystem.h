#pragma once

#include "../Engine/Entity/Entity.h"
#include "../Engine/Entity/IEntitySystem.h"

#include "KinematicsComponent.h"

class MovementSystem : public IEntitySystem<KinematicsComponent>
{
public:
	MovementSystem() {};
	virtual ~MovementSystem() {};

	virtual void processEntities();
private:
};