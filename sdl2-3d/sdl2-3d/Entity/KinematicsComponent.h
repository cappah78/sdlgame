#pragma once

#include "../Engine/Entity/IComponent.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

class KinematicsComponent : public IComponent
{
public:
	KinematicsComponent();
	virtual ~KinematicsComponent() override;
	
	glm::quat orientation;
	glm::vec3 position;
	glm::vec3 velocity;

	glm::quat targetOrientation;
	glm::vec3 targetVelocity;
private:
};