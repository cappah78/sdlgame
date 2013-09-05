#include "EntityFactory.h"

coment::Entity& EntityFactory::createPlayer(coment::World& world, float x, float y, float z)
{
	EntityBuilder b(world, world.createEntity());

	return b.position(x, y, z).velocity(0, 0, 0).getEntity();
}