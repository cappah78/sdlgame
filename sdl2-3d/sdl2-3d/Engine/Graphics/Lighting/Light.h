#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>

class Light 
{
public:
	Light(glm::vec3& color = glm::vec3(1, 1, 1)) 
		: m_color(color)
		, m_enabled(true)
		, m_updated(false)
	{};
	virtual ~Light() {};

	glm::vec3 m_color;
	bool m_enabled;
	/* False if this light should be be updated in the uniform buffer */
	bool m_updated;
};

#endif //LIGHT_H_