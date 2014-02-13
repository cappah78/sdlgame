#pragma once

#include "../Engine/Entity/IComponent.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

class KinematicsComponent : public IComponent
{
public:
	KinematicsComponent();
	virtual ~KinematicsComponent() override;
	
	glm::quat m_orientation;
	glm::vec3 m_position;
	glm::vec3 m_velocity;

	glm::quat m_targetOrientation;
	glm::vec3 m_targetVelocity;

	float m_acceleration;
	float m_turnSpeed;
private:
};