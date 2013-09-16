#ifndef GBUFFER_H_
#define GBUFFER_H_

typedef unsigned int GLuint;
typedef int GLint;

#include <glm/glm.hpp>

class Camera;

class GBuffer
{
public:
	struct Transform {
		glm::mat4 MVPMatrix;			// modelview-projection matrix
		glm::mat4 MVMatrix;
		glm::mat4 PMatrix;
	};


	GBuffer();
	~GBuffer();

	void loadShaders();
	void setupGBuffer();

	void drawBuffer();

	void use(const Camera& camera);
	void setupShadows();
	void renderLights();

private:
	void setupUniforms();

	Transform transform;			// transformation data
	GLuint transformUB;				// uniform buffer for the transformation

	GLuint GBufferPPL;				// G-Buffer program pipeline
	GLint u_transformBlockLoc;
	GLint u_positionBufferLoc;
	GLint u_depthBufferLoc;
	GLint u_normalBufferLoc;
	GLint u_shadowMapArrayLoc;

	GLuint lightPassPPL;
	GLuint lightPPL;				// light program pipeline

	GLuint shadowSinglePPL;			// single shadow program pipeline
	GLint lightIDUniformLoc;		// location of the lightID uniform

	GLuint shadowMultiPPL;			// multi shadow program pipeline

	GLuint positionGBT;				// position texture for G-buffer
	GLuint normalGBT;				// normal texture for G-buffer
	GLuint depthGBT;				// depth texture for G-buffer
	GLuint gBuffer;					// G-buffer framebuffer

	GLuint shadowArrayTex;			// shadow map texture cube array
	GLuint shadowFB;				// shadow rendering framebuffer
};

#endif //GBUFFER_H_