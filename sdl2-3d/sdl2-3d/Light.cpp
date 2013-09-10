#include "Light.h"

const unsigned int DEPTH_TEXTURE_SIZE = 512;

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