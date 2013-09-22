#ifndef m_positionH_
#define m_positionH_

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

#endif //m_positionH_
