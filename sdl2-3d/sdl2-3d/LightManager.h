#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Camera;
class Light;


struct LightData {
	glm::vec4 position;				// light position
	glm::vec4 color;				// light color
};

struct LightTransform {
	glm::mat4 VPMatrix;				// light's shadow rendering view-projection matrix
};

class LightManager
{
public:
	LightManager();
	~LightManager();

	void update(const Camera& camera);
	Light* createPointLight(glm::vec3& position, glm::vec3& color, float linearAttenuation);

	static const int MAX_LIGHTS = 1;

private:
	void sortLights(const Camera& camera);
	void updateBlock(const Camera& camera);
	void updateShadowMaps();

	std::vector<Light*> lightObjects;


	GLuint lightUB;
	LightData lights[MAX_LIGHTS];

	GLuint lightTransformUB;
	LightTransform lightTransform[MAX_LIGHTS];
};

#endif //LIGHTMANAGER_H_