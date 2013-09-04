#include "EntityBuilder.h"

using namespace component;

EntityBuilder::EntityBuilder(coment::World& world_, coment::Entity& e)
	: world(world_)
	, entity(e)
{

}

EntityBuilder::~EntityBuilder()
{

}

EntityBuilder& EntityBuilder::position(float x, float y, float z)
{
	Position* p = world.addComponent<Position>(entity);
	p->x = x;
	p->y = y;
	p->z = z;

	return *this;
}


EntityBuilder& EntityBuilder::velocity(float x, float y, float z)
{
	Velocity* v = world.addComponent<Velocity>(entity);
	v->x = x;
	v->y = y;
	v->z = z;

	return *this;
}

coment::Entity& EntityBuilder::getEntity()
{
	return entity;
}