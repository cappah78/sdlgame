#include "PerspectiveCamera.h"

#include <glm\gtc\matrix_transform.hpp>

const float FOVY = 80.0f;
const float NEAR = 0.25f;
const float FAR = 100.0f;

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction)
	: Camera(position, direction)
	, m_fieldOfView(FOVY)
	, m_near(NEAR)
	, m_far(FAR)
{
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_viewportWidth / m_viewportHeight, m_near, m_far);
}

PerspectiveCamera::PerspectiveCamera(float fovy, float width, float height)
	: Camera(width, height)
	, m_fieldOfView(fovy)
{
	m_projectionMatrix = glm::perspective(fovy, width / height, NEAR, FAR);
}

void PerspectiveCamera::setFieldOfView(float fovy)
{
	m_fieldOfView = fovy;
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_viewportWidth / m_viewportHeight, m_near, m_far);
}

void PerspectiveCamera::setNearFar(float near, float far)
{
	m_near = near;
	m_far = far;
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_viewportWidth / m_viewportHeight, near, far);
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::resize(int width, int height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	m_projectionMatrix = glm::perspective(m_fieldOfView, (float) width / height, NEAR, FAR);
}