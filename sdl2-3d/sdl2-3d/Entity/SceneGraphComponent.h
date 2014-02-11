#pragma once

#include "../Engine/Entity/IComponent.h"

#include <glm\glm.hpp>
#include <vector>

class SceneGraphComponent : public IComponent
{
public:
	SceneGraphComponent() {};
	virtual ~SceneGraphComponent() {};

	glm::mat4 transform;
	std::vector<unsigned int> childIndici;
	bool isRoot;
	bool isDirty;
};