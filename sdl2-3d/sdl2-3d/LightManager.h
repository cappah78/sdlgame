#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

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
	std::vector<const Light*>& getLights();
	Light* createPointLight(glm::vec3& position, glm::vec3& color, float linearAttenuation);

private:
	void sortLights(const Camera& camera);

	std::vector<const Light*> lightObjects;
};

#endif //LIGHTMANAGER_H_