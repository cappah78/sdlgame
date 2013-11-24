#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_

#include <vector>
#include <glm/glm.hpp>

#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

typedef unsigned int GLuint;
typedef std::vector<PointLight*> PointLights;
typedef std::vector<DirectionalLight*> DirectionalLights;
typedef std::vector<SpotLight*> SpotLights;
typedef std::vector<AmbientLight*> AmbientLights;

class Camera;

class LightManager
{
public:
	LightManager();
	~LightManager();

	void update(const Camera& camera);

	PointLight* addLight(PointLight* pointLight);
	DirectionalLight* addLight(DirectionalLight* directionalLight);
	SpotLight* addLight(SpotLight* spotLight);
	AmbientLight* addLight(AmbientLight* ambientLight);

	void removeLight(PointLight* pointLight);
	void removeLight(DirectionalLight* directionalLight);
	void removeLight(SpotLight* spotLight);
	void removeLight(AmbientLight* ambientLight);

	PointLights& getPointLights();
	DirectionalLights& getDirectionalLights();
	SpotLights& getSpotLights();
	AmbientLights& getAmbientLights();

private:
	void sortLights(const Camera& camera);

	PointLights m_pointLights;
	DirectionalLights m_directionalLights;
	SpotLights m_spotLights;
	AmbientLights m_ambientLights;
};

#endif //LIGHT_MANAGER_H_
