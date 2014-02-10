#ifndef ENTITY_WORLD_H_
#define ENTITY_WORLD_H_

#include "IComponent.h"
#include "Entity.h"
#include "IBehaviorSystem.h"

#include <unordered_map>
#include <vector>

class EntityWorld
{

public:
	EntityWorld();
	~EntityWorld();

	void addComponent(const IComponent& component);

	template <typename COMPONENTTYPE>
	COMPONENTTYPE getComponent(const ComponentID& componentID);
private:
	std::vector<std::pair<IBehaviorSystem, ComponentID>> m_systemMap;
	std::unordered_map<ComponentID, ComponentList> m_componentMap;
};

#endif //ENTITY_WORLD_H_