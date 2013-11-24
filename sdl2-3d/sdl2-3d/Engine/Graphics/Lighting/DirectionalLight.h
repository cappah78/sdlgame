#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_

#include "Light.h"

struct DirectionalLightData
{
	glm::vec4 direction;
	glm::vec4 color;
};

class DirectionalLight : public Light 
{
public:
	//TODO: bound checking, getters/setters with m_updated etc. in .cpp
	DirectionalLight(glm::vec3& direction, glm::vec3& color, float intensity)
		: Light(color)
		, m_direction(direction)
		, m_intensity(intensity)
	{};
	~DirectionalLight() {};

	glm::vec3 m_direction;
	float m_intensity;

	inline DirectionalLightData getData() 
	{
		return { glm::vec4(m_direction, 0.0), glm::vec4(m_enabled ? m_color * m_intensity : glm::vec3(0), 0.0) };
	};
private:
};

#endif //DIRECTIONAL_LIGHT_H_