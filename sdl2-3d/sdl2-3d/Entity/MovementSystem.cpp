#include "MovementSystem.h"

#include <glm\gtc\matrix_transform.hpp>

inline void interpolateTo(float& value, const float& to, const float& changeAmount)
{
	//TODO: factor in float epsilon.
	if (value < to)
	{
		value += changeAmount;
		if (value > changeAmount)
			value = to;
	}
	else if (value > to)
	{
		value -= changeAmount;
		if (value < changeAmount)
			value = to;
	}
}

void MovementSystem::processEntities(float deltaSec)
{
	for (KinematicsComponent& component : m_componentList)
	{
		glm::vec3& velocity = component.m_velocity;
		glm::vec3& targetVelocity = component.m_targetVelocity;
		float accelerationAmount = component.m_acceleration * deltaSec;

		interpolateTo(velocity.x, targetVelocity.x, accelerationAmount);
		interpolateTo(velocity.y, targetVelocity.y, accelerationAmount);
		interpolateTo(velocity.z, targetVelocity.z, accelerationAmount);

		component.m_position += component.m_velocity;

		//TODO: rotateTowardsTarget
		float cosAngle = glm::dot(component.m_orientation, component.m_targetOrientation);
	}
}