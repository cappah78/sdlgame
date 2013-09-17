#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>

class Light
{
	friend class LightManager;
	friend class GBuffer;
public:
	glm::vec3 position;
	glm::vec3 color;
	float linearAttenuation;
	bool isEnabled;

	void setEnabled(bool enabled);
	void setPosition(glm::vec3 position);
	void setColor(glm::vec3 color);
	void setLinearAttenuation(float att);
private:
	Light(glm::vec3 position, glm::vec3 color, float linearAttenuation);
	~Light();
};

#endif //LIGHT_H_