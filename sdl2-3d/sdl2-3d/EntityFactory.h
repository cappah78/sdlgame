#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include "EntityBuilder.h"

class EntityFactory
{
public:
	static coment::Entity& createPlayer(coment::World& world, float x, float y, float z);
};

#endif //ENTITYFACTORY_H_