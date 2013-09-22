#include "MovementSystem.h"

#include <iostream>
#include <stdio.h>

#include "Position.h"
#include "Velocity.h"

namespace entitysystem
{
	MovementSystem::MovementSystem()
	{

	}

	void MovementSystem::registerComponents()
	{
		registerComponent<component::Position>();
		registerComponent<component::Velocity>();
	}

	void MovementSystem::process(const coment::Entity& e)
	{
		component::Position* p = _world->getComponent<component::Position>(e);
		component::Velocity* v = _world->getComponent<component::Velocity>(e);

		p->x += v->x * m_delta;
		p->y += v->y * m_delta;
		p->z += v->z * m_delta;

		//std::cout << "pos: " << p->x << ":" << p->y << ":" << p->z << ":" << std::endl;
	}
}