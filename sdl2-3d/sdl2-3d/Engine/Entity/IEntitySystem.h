#pragma once

#include <vector>

template <typename COMPONENT>
class IEntitySystem
{
public:
	IEntitySystem();
	virtual ~IEntitySystem() {};

	virtual void processEntities();

protected:
	std::vector<COMPONENT> m_componentList;
private:
};