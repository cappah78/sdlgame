#pragma once

#include "IComponent.h"
#include <vector>

template <typename UPDATE, typename WITH>
class ComponentUpdateSystem
{
public:
	ComponentUpdateSystem() {};
	virtual ~ComponentUpdateSystem() {};
	virtual void updateComponents(std::vector<UPDATE>& update, const std::vector<WITH>& with) = 0;

private:
};