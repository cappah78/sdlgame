#ifndef LIGHT_H_
#define LIGHT_H_

#include <gl/glew.h>
#include <glm/glm.hpp>

class Light
{
	friend class LightManager;
public:
	bool isEnabled;
	glm::vec3 position;
	glm::vec3 color;
	float linearAttenuation;
private:
	Light(glm::vec3& position, glm::vec3& color, float linearAttenuation);
	~Light();

	GLuint depthTexture;
	GLuint depthFbo;
};

#endif //LIGHT_H_