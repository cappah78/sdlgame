#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm\glm.hpp>

class Camera
{
public:
	Camera(float viewportWidth, float viewportHeight);
	Camera(glm::vec3 position, glm::vec3 direction = glm::vec3(0, 0, -1));
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

	void setPosition(float x, float y, float z);

	float getX();
	float getY();
	float getZ();
	/** rotation on the x/y plane from -180 to 180, 0 == up */
	float getRotationRadXY();
	/** rotation on the x/z plane from -180 to 180, 0 == up */
	float getRotationRadXZ();
	/** rotation on the y/z plane from -180 to 180, 0 == up */
	float getRotationRadYZ();

	glm::mat4 m_combinedMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;

	float m_viewportWidth;
	float m_viewportHeight;
};

#endif //CAMERA_H_