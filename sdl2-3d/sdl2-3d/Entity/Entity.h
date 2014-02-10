#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

class Entity
{
public:
	Entity();
	~Entity();
	Entity(const Entity& copy) = delete;
private:
	std::vector<bool> m_componentBits;
	std::vector<bool> m_behaviorBits;
};

#endif ENTITIY_H_