#ifndef UNIFORM_BUFFER_STRUCTS
#define UNIFORM_BUFFER_STRUCTS

#include <glm\glm.hpp>

struct CameraTransform {
	glm::mat4 VPMatrix;
	glm::mat4 VMatrix;
	glm::mat4 PMatrix;
};

struct VertexTransform{
	glm::mat4 V1MVPMatrix;
	glm::mat4 V2MVPMatrix;
	glm::mat4 V3MVPMatrix;
	glm::mat4 V4MVPMatrix;
};

struct LightTransform {
	glm::mat4 VPMatrix;;
};

#endif //UNIFORM_BUFFER_STRUCTS