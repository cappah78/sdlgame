#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>

class Light
{
public:
	friend class LightManager;
	friend class GBuffer;

	void setEnabled(bool enabled);
	void setPosition(glm::vec3 position);
	void setColor(glm::vec3 color);
	void setLinearAttenuation(float att);
	void update();
	bool isUpdated();
	void setUpdated(bool updated);

	glm::vec3 m_position;
	glm::vec3 m_color;
	glm::vec3 m_direction;

	float m_spotRadius;
	float m_linearAttenuation;
	float m_spotDropoff;
	bool m_isEnabled;


private:
	Light(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float linearAttenuation, float spotRadius, float spotDropoff);
	~Light();

	bool m_isUpdated;
};

#endif //LIGHT_H_