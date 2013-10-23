#ifndef GBUFFER_H_
#define GBUFFER_H_

#include "Light.h"
#include "LightManager.h"
#include <glm/glm.hpp>

typedef int GLint;
typedef unsigned int GLuint;

class Camera;

class GBuffer
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


	GBuffer();
	~GBuffer();

	void loadShaders();
	void setupGBuffer();

	void drawBuffer();

	void use(const Camera& camera);
	void setupShadows();

	void updateLights(const Camera& camera, LightManager& LightManager);

	static const int MAX_LightS = 10;

private:
	void setupUniforms();

	int m_numLights;

	CameraTransform m_cameraTransform;			// transformation data
	GLuint m_cameraTransformUB;				// uniform buffer for the transformation

	LightData m_LightData[MAX_LightS];
	GLuint m_LightDataUB;

	LightTransform m_LightTransforms[MAX_LightS];
	GLuint m_LightTransformUB;

	GLuint m_gBufferProgram;				// G-Buffer program pipeline
	GLint m_transformBlockLoc;
	GLint m_colorBufferLoc;
	GLint m_depthBufferLoc;
	GLint m_normalBufferLoc;
	GLint m_shadowMapArrayLoc;

	GLuint m_LightPassProgram;

	GLuint m_forwardShaderProgram;

	GLint m_numLightsLoc;

	GLuint m_shadowSingleProgram;			// single shadow program pipeline
	GLint m_LightIDUniformLoc;		// location of the LightID uniform

	GLuint m_shadowMultiProgram;			// multi shadow program pipeline
	GLuint m_shadowDepthTex;
	GLuint m_shadowFBO;

	GLuint m_colorBufferTex;				// position texture for G-buffer
	GLuint m_normalBufferTex;				// normal texture for G-buffer
	GLuint m_depthBufferTex;				// depth texture for G-buffer
	GLuint m_gBuffer;					// G-buffer framebuffer
};

#endif //GBUFFER_H_