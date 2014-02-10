#ifndef ICOMPONENT_H_
#define ICOMPONENT_H_

#include <vector>

typedef unsigned int ComponentID;
typedef std::vector<IComponent> ComponentList;

class IComponent
{
public:
	IComponent();
	virtual ~IComponent();
	

	static unsigned int s_componentID;
};

#endif ICOMPONENT_H_