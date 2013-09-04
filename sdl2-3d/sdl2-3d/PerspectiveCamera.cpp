#include "PerspectiveCamera.h"

const float FOVY = 80;
const float DEFAULT_ASPECTRATIO = 1.0f;
const float NEAR = 0.5f;
const float FAR = 1000.0f;

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction)
	: Camera(position, direction)
{
	projectionMatrix = glm::perspective(FOVY, DEFAULT_ASPECTRATIO, NEAR, FAR);
	combinedMatrix = projectionMatrix * viewMatrix;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::resize(int width, int height)
{
	projectionMatrix = glm::perspective(FOVY, (float) width / height, NEAR, FAR);
}