#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_

#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Camera;
class Light;

class LightManager
{
public:
	LightManager();
	~LightManager();

	void update(const Camera& camera);
	std::vector<Light*>& getLights();
	Light* createPointLight(glm::vec3& position, glm::vec3& direction, glm::vec3& color, float linearAttenuation, float spotRadius, float spotExponent);

private:
	void sortLights(const Camera& camera);

	std::vector<Light*> m_lightObjects;
};

#endif //LIGHT_MANAGER_H_