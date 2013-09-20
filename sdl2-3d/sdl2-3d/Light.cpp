#include "Light.h"

const unsigned int DEPTH_TEXTURE_SIZE = 512;

Light::Light(glm::vec3 position_, glm::vec3 direction_, glm::vec3 color_, float linearAttenuation_, float spotRadius_, float spotDropoff_)
	: position(position_)
	, color(color_)
	, direction(direction_)
	, linearAttenuation(linearAttenuation_)
	, spotRadius(spotRadius_)
	, spotDropoff(spotDropoff_)
	, isEnabled(true)
{
	
}

Light::~Light()
{

}

void Light::setEnabled(bool enabled)
{
	isEnabled = enabled;
}

void Light::setPosition(glm::vec3 position_)
{
	position = position_;
}

void Light::setColor(glm::vec3 color_)
{
	color = color_;
}

void Light::setLinearAttenuation(float att)
{
	linearAttenuation = att;
}