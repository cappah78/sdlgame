#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Camera;
class Light;

const int maxLights = 10;

struct LightProperties
{
	glm::vec4 position;
	glm::vec4 color;
	glm::mat4 lightVPMat;
};

struct LightBlock
{
	glm::vec3 eyeDirection;
	float padding;
	LightProperties lights[maxLights];
};

class LightManager
{
public:
	LightManager(GLuint shaderId);
	~LightManager();

	void update(const Camera& camera);
	Light* createPointLight(glm::vec3& position, glm::vec3& color, float linearAttenuation);

	static const char* U_LIGHTBLOCK_NAME;
	static const int LIGHTBLOCK_INDEX;

private:
	void sortLights(const Camera& camera);
	void updateBlock(const Camera& camera);
	void updateShadowMaps();

	std::vector<Light*> lights;
	LightBlock lightBlock;

	GLuint u_lightBlock;
	GLuint lightUniformBuffer;

	GLuint shadowArrayTex;
	GLuint shadowFb;
};

#endif //LIGHTMANAGER_H_