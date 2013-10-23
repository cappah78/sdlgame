#ifndef AMBIENT_LIGHT_H_
#define AMBIENT_LIGHT_H_

#include "Light.h"

class AmbientLight : public Light 
{
public:
	//TODO: bound checking, getters/setters with m_updated etc. in .cpp
	AmbientLight(glm::vec3& color)
		: Light(color)
	{};
	~AmbientLight() {};
private:
};

#endif //AMBIENT_LIGHT_H_