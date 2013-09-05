#ifndef LIGHT_H_
#define LIGHT_H_

#include <gl/glew.h>
#include <glm/glm.hpp>

class Light
{
	friend class LightManager;
protected:
	bool isEnabled;
	glm::vec3 position;
	glm::vec3 color;
	float linearAttenuation;
private:
	Light(glm::vec3& position, glm::vec3& color, float linearAttenuation);
	~Light();
};

#endif //LIGHT_H_