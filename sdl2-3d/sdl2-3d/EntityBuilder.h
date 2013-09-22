#ifndef ENTITYBUILDER_H_
#define ENTITYBUILDER_H_

#include "coment\World.h"

class EntityBuilder
{
public:
	EntityBuilder(coment::World& world, coment::Entity& e);
	~EntityBuilder();

	EntityBuilder& position(float x, float y, float z);
	EntityBuilder& velocity(float x, float y, float z);
	coment::Entity& getEntity();
private:
	coment::World& m_world;
	coment::Entity& m_entity;
};

#endif //ENTITYBUILDER_H_