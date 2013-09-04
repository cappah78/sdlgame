#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "MathUtils.h"

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 direction);
	virtual ~Camera();
	virtual void translate(float x, float y, float z);
	/** Translates the camera position relative to the view of the camera */
	virtual void translateRelative(float x, float y, float z);
	virtual void rotate(float angle, glm::vec3 axis);
	virtual void rotateRelative(float xRot, float yRot);
	virtual void lookAt(float x, float y, float z);
	virtual void lookAtDir(glm::vec3 dir);
	virtual void resize(int width, int height) {};
	virtual void update();

	/** rotation on the x/y plane from -180 to 180, 0 == up */
	float getRotationRadXY();
	/** rotation on the x/z plane from -180 to 180, 0 == up */
	float getRotationRadXZ();
	/** rotation on the y/z plane from -180 to 180, 0 == up */
	float getRotationRadYZ();

	glm::mat4 combinedMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
};

#endif //CAMERA_H_