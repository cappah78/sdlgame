#ifndef POSITION_H_
#define POSITION_H_

#include "coment\Component.h"

namespace component 
{
	struct Position : public coment::Component
	{
		Position() : x(0), y(0), z(0) {};
		Position(float x, float y, float z) : x(x), y(y), z(z) {};
		~Position() {};
		float x, y, z;
	};
}

#endif //POSITION_H_