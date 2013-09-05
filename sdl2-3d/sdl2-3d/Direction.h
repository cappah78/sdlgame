#ifndef DIRECTION_H_
#define DIRECTION_H_

#include "coment\Component.h"
#include <glm\glm.hpp>

namespace component
{
	struct Direction : public coment::Component
	{
		Direction(float x, float y, float z) : dir(x, y, z) {};
		Direction() : dir(1, 0, 0) {};
		glm::vec3 dir;
	};
}

#endif //DIRECTION_H_