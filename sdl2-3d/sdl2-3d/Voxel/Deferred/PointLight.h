#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include <glm\glm.hpp>

struct PointLight
{
	PointLight(const glm::vec3& position, const glm::vec3& color = glm::vec3(1, 1, 1), float diffuse = 10.0f, float ambient = 0.05f, float lin = 0.0f, float con = 0.0f, float exp = 0.3f)
	: position(position), color(color), diffuseIntensity(diffuse), ambientIntensity(ambient), linAtten(lin), constAtten(con), expAtten(exp)
	{}

	glm::vec3 color;
	glm::vec3 position;
	float diffuseIntensity;
	float ambientIntensity;
	float linAtten;
	float constAtten;
	float expAtten;
};

#endif //POINT_LIGHT_H_