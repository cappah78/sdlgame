#ifndef FORWARD_SHADER_H_
#define FORWARD_SHADER_H_

#include <glm/glm.hpp>

#include "Camera.h"
#include "LightManager.h"

const int MAX_LIGHTS = 10;

typedef int GLint;
typedef unsigned int GLuint;

class ForwardShader
{
public:
	struct CameraTransform {
		glm::mat4 VPMatrix;
		glm::mat4 VMatrix;
		glm::mat4 PMatrix;
	};

	struct LightData {
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 color;
	};

	struct LightTransform {
		glm::mat4 VPMatrix;;
	};

	ForwardShader();
	~ForwardShader();
	void use(const Camera& camera);
	void updateLights(const Camera& camera, LightManager& lightManager);
	void generateShadowMaps();
	void finishShadowMaps();

private:
	void setupUniforms();
	void setupBuffers();
	void setupShadowFramebuffer();

	GLuint m_forwardShaderProgram;

	int m_numLights;
	GLint m_numLightsLoc;

	GLuint m_shadowProgram;			// multi shadow program pipeline
	GLuint m_shadowArrayTex;
	GLuint m_shadowFBO;

	CameraTransform m_cameraTransform;			// transformation data
	GLuint m_cameraTransformBuffer;				// uniform buffer for the transformation

	LightData m_lightData[MAX_LIGHTS];
	GLuint m_lightDataBuffer;

	LightTransform m_lightTransforms[MAX_LIGHTS];
	GLuint m_lightTransformBuffer;
};

#endif //FORWARD_SHADER_H_