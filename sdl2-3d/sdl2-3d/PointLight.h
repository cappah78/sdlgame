#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "Light.h"

/* Equivalent to uniform buffer struct, includes padding */
struct PointLightData
{
	glm::vec4 position;
	glm::vec4 color;
};

class PointLight : public Light
{
public:
	//TODO: bound checking, getters/setters with m_updated etc. in .cpp
	PointLight(glm::vec3& position, glm::vec3& color, float range)
		: Light(color)
		, m_position(position)
		, m_range(range)
	{};
	~PointLight() {};

	glm::vec3 m_position;
	float m_range;

	inline PointLightData getData()
	{
		PointLightData l = { glm::vec4(m_position, 1.0f), glm::vec4(m_color * m_range, 1.0) }; // premultiplying color with range.
		return l;
	};
private:
};

#endif //POINT_LIGHT_H_