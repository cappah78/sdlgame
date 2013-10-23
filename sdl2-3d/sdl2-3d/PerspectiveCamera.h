#ifndef PERSPECTIVE_CAMERA_H_
#define PERSPECTIVE_CAMERA_H_

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 position, glm::vec3 direction = glm::vec3(1, 0, 0));
	PerspectiveCamera(float fovy, float width, float height);
	~PerspectiveCamera();

	void setFieldOfView(float fovy);
	void setNearFar(float near, float far);
	void resize(int width, int height);

	float getNear() const { return m_near; };
	float getFar() const { return m_far; };
	float getFov() const { return m_fieldOfView; };

private:
	float m_fieldOfView;
	float m_near;
	float m_far;
};

#endif //PERSPECTIVE_CAMERA_H_
