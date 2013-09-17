#ifndef GBUFFER_H_
#define GBUFFER_H_

#include "Light.h"

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
		glm::vec4 color;
	};


	GBuffer();
	~GBuffer();

	void loadShaders();
	void setupGBuffer();

	void drawBuffer();

	void use(const Camera& camera);
	void setupShadows();

	void updateLights(const Camera& camera, LightManager& lightManager);

	static const int MAX_LIGHTS = 10;

private:
	void setupUniforms();

	int numLights;

	CameraTransform cameraTransform;			// transformation data
	GLuint cameraTransformUB;				// uniform buffer for the transformation

	LightData lightData[MAX_LIGHTS];
	GLuint lightDataUB;

	GLuint GBufferPPL;				// G-Buffer program pipeline
	GLint u_transformBlockLoc;
	GLint u_colorBufferLoc;
	GLint u_depthBufferLoc;
	GLint u_normalBufferLoc;
	GLint u_shadowMapArrayLoc;

	GLuint lightPassPPL;

	GLuint shadowSinglePPL;			// single shadow program pipeline
	GLint lightIDUniformLoc;		// location of the lightID uniform

	GLuint shadowMultiPPL;			// multi shadow program pipeline

	GLuint colorGBT;				// position texture for G-buffer
	GLuint normalGBT;				// normal texture for G-buffer
	GLuint depthGBT;				// depth texture for G-buffer
	GLuint gBuffer;					// G-buffer framebuffer

	GLuint shadowArrayTex;			// shadow map texture cube array
	GLuint shadowFB;				// shadow rendering framebuffer
};

#endif //GBUFFER_H_