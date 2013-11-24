#ifndef SPOT_LIGHT_H_
#define SPOT_LIGHT_H_

#include "Light.h"
#include <glm\glm.hpp>

struct SpotLightData
{
	glm::vec4 positionAndAngle;
	glm::vec4 directionAndDropoff;
	glm::vec4 color;
};

class SpotLight : public Light 
{
public:
	//TODO: bound checking, getters/setters with m_updated etc. in .cpp
	SpotLight(glm::vec3& position, glm::vec3& direction, glm::vec3& color, float range, float coneAngle, float coneDropff)
		: Light(color)
		, m_position(position)
		, m_direction(direction)
		, m_range(range)
		, m_coneAngle(coneAngle)
		, m_coneDropoff(coneDropff)
	{};
	~SpotLight() {};

	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_range;
	float m_coneAngle;
	float m_coneDropoff;

	inline SpotLightData getData()
	{
		SpotLightData l = { glm::vec4(m_position, m_coneAngle), glm::vec4(m_direction, m_coneDropoff), glm::vec4(m_color * m_range, 1.0) };
		return l;
	};
private:
};

#endif //SPOT_LIGHT_H_