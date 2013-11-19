#ifndef UNIFORM_BUFFER_STRUCTS
#define UNIFORM_BUFFER_STRUCTS

#include <glm\glm.hpp>

struct CameraTransform {
	glm::mat4 VPMatrix;
	glm::mat4 VMatrix;
	glm::mat4 PMatrix;
};

struct LightTransform {
	glm::mat4 VPMatrix;;
};

#endif //UNIFORM_BUFFER_STRUCTS