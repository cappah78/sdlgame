#include "Light.h"

const unsigned int DEPTH_m_textureSIZE = 512;

Light::Light(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float linearAttenuation, float spotRadius, float spotDropoff)
	: m_position(position)
	, m_color(color)
	, m_direction(direction)
	, m_linearAttenuation(linearAttenuation)
	, m_spotRadius(spotRadius)
	, m_spotDropoff(spotDropoff)
	, m_isEnabled(true)
	, m_isUpdated(false)
{
	
}

Light::~Light()
{

}

void Light::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
}

void Light::setPosition(glm::vec3 position)
{
	m_position = position;
}

void Light::setColor(glm::vec3 color)
{
	m_color = color;
}

void Light::setLinearAttenuation(float att)
{
	m_linearAttenuation = att;
}

void Light::update()
{
	m_isUpdated = false;
}

bool Light::isUpdated()
{
	return m_isUpdated;
}

void Light::setUpdated(bool updated)
{
	m_isUpdated = updated;
}