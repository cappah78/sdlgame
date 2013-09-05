#ifndef VELOCITY_H_
#define VELOCITY_H_

#include "coment\Component.h"

namespace component 
{
	struct Velocity : public coment::Component
	{
		Velocity() : x(0), y(0), z(0) {};
		Velocity(float x, float y, float z) : x(x), y(y), z(z) {};
		~Velocity() {};
		float x, y, z;
	};
}

#endif //VELOCITY_H_