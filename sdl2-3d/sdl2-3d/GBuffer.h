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
	Transform transform;			// transformation data
	GLuint transformUB;				// uniform buffer for the transformation

	GLuint GBufferVSP;				// G-Buffer vertex shader program
	GLuint GBufferFSP;				// G-Buffer fragment shader program
	GLuint GBufferPPL;				// G-Buffer program pipeline

	GLuint lightPassVSP;
	GLuint lightPassGSP;
	GLuint lightPassFSP;
	GLuint lightPassPPL;

	GLuint lightVSP;				// light vertex shader program
	GLuint lightGSP;				// light geometry shader program
	GLuint lightFSP;				// light fragment shader program
	GLuint lightPPL;				// light program pipeline

	GLuint shadowSingleVSP;			// single shadow vertex shader program
	GLuint shadowSinglePPL;			// single shadow program pipeline
	GLint lightIDUniformLoc;		// location of the lightID uniform

	GLuint shadowMultiVSP;			// multi shadow vertex shader program
	GLuint shadowMultiGSP;			// multi shadow geometry shader program
	GLuint shadowMultiPPL;			// multi shadow program pipeline

	GLuint positionGBT;				// position texture for G-buffer
	GLuint normalGBT;				// normal texture for G-buffer
	GLuint depthGBT;				// depth texture for G-buffer
	GLuint gBuffer;					// G-buffer framebuffer

	GLuint shadowArrayTex;			// shadow map texture cube array
	GLuint shadowFB;				// shadow rendering framebuffer
};

#endif //GBUFFER_H_