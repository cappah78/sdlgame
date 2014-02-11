#pragma once

#include "../Utils/HashCode.h"

class IComponent
{
public:
	IComponent();
	virtual ~IComponent() {};

	static HashCode s_componentID;
private:
	
};