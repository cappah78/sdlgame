#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 position, glm::vec3 direction = glm::vec3(0, 0, -1));
	~PerspectiveCamera();

	void resize(int width, int height);
private:

};

#endif //PERSPECTIVECAMERA_H_