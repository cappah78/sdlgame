#pragma once

#include "SceneGraphUpdateSystem.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\matrix_operation.hpp>

void SceneGraphUpdateSystem::updateComponents(std::vector<SceneGraphComponent>& update, const std::vector<KinematicsComponent>& with)
{
	assert(update.size() <= with.size());
	for (unsigned int i = 0; i < update.size(); ++i)
	{
		SceneGraphComponent& component = update[i];
		if (!component.isRoot)
			continue;
		if (component.isDirty)
		{
			const KinematicsComponent& kinematic = with[i];
			
		}
	}

}