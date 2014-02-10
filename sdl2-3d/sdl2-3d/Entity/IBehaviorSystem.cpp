#include "IBehaviorSystem.h"

#include <glm\glm.hpp>

IBehavior2System<glm::vec3, glm::vec3>::IBehavior2System()
{
	
}

void IBehavior2System<glm::vec3, glm::vec3>::update(float deltaSec)
{
	for (int i = 0; i < m_componentList.size(); ++i)
	{
		ComponentPair& p = m_componentList[i];
		p.component1 += p.component2 * deltaSec;
	}
}