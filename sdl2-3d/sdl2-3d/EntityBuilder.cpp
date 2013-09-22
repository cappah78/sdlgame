#include "EntityBuilder.h"

#include "Position.h"
#include "Velocity.h"

using namespace component;

EntityBuilder::EntityBuilder(coment::World& m_world, coment::Entity& e)
	: m_world(m_world)
	, m_entity(e)
{

}

EntityBuilder::~EntityBuilder()
{

}

EntityBuilder& EntityBuilder::position(float x, float y, float z)
{
	Position* p = m_world.addComponent<Position>(m_entity);
	p->x = x;
	p->y = y;
	p->z = z;

	return *this;
}


EntityBuilder& EntityBuilder::velocity(float x, float y, float z)
{
	Velocity* v = m_world.addComponent<Velocity>(m_entity);
	v->x = x;
	v->y = y;
	v->z = z;

	return *this;
}

coment::Entity& EntityBuilder::getEntity()
{
	return m_entity;
}