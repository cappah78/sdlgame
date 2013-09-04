#include "DeltaProcessingSystem.h"

#include <iostream>
#include <stdio.h>

namespace entitysystem
{
	void DeltaProcessingSystem::processEntities(std::vector<coment::Entity>& entities)
	{
		delta = _world->getDelta();

		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			process(entities[i]);
		}
	}
}