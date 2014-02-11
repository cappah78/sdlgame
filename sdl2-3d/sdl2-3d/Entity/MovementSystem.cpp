#include "MovementSystem.h"

#include <glm\gtc\matrix_transform.hpp>

void MovementSystem::processEntities()
{
	for (KinematicsComponent& component : m_componentList)
	{
		component.position += component.velocity;
	}
}