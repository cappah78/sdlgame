#include "Light.h"

Light::Light(glm::vec3& position_, glm::vec3& color_, float linearAttenuation_)
	: position(position_)
	, color(color_)
	, linearAttenuation(linearAttenuation_)
	, isEnabled(true)
{

}

Light::~Light()
{

}