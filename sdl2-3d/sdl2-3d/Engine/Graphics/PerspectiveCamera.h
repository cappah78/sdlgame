#ifndef PERSPECTIVE_CAMERA_H_
#define PERSPECTIVE_CAMERA_H_

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float viewportWidth, float viewportHeight, float verticalFOV, float near, float far);
	~PerspectiveCamera();

	void setFieldOfView(float fovy);
	void resize(int width, int height) override;
	void setNearFar(float near, float far);

	float getFov() const { return m_fieldOfView; };

private:
	float m_fieldOfView;
};

#endif //PERSPECTIVE_CAMERA_H_
