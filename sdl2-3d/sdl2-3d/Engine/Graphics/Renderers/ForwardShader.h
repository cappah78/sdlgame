#ifndef FORWARD_SHADER_H_
#define FORWARD_SHADER_H_

//TODO: Rewrite lighting

#include "UniformBufferStructs.h"
#include <glm/glm.hpp>

#include "..\Camera.h"
#include "..\Lighting\LightManager.h"

/**
Shader with simple Phong lighting
*/
class ForwardShader
{
public:
	typedef int GLint;
	typedef unsigned int GLuint;

	static const unsigned int MAX_POINT_LIGHTS = 5;
	static const unsigned int MAX_SPOT_LIGHTS = 5;
	static const unsigned int MAX_DIRECTIONAL_LIGHTS = 5;

	static const unsigned int CAMERA_TRANSFORM_BINDING_POINT = 0;
	static const unsigned int POINT_LIGHT_DATA_BINDING_POINT = 1;
	static const unsigned int SPOT_LIGHT_DATA_BINDING_POINT = 2;
	static const unsigned int DIRECTIONAL_LIGHT_DATA_BINDING_POINT = 3;

	ForwardShader();
	~ForwardShader();
	void use(const Camera& camera);
	void updateLights(const Camera& camera, LightManager& LightManager);


private:
	void setupUniforms();
	void setupBuffers();

	/** Program ID*/
	GLuint m_forwardShaderProgram;

	CameraTransform m_cameraTransform;			// transformation data
	GLuint m_cameraTransformBuffer;				// uniform buffer for the transformation

	PointLightData m_pointLightData[MAX_POINT_LIGHTS];
	GLuint m_pointLightDataBuffer;

	SpotLightData m_spotLightData[MAX_SPOT_LIGHTS];
	GLuint m_spotLightDataBuffer;

	DirectionalLightData m_directionalLightData[MAX_DIRECTIONAL_LIGHTS];
	GLuint m_directionalLightDataBuffer;

	GLint m_ambientLightUniformLoc;
};

#endif //FORWARD_SHADER_H_