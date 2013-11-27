#include "PerspectiveCamera.h"

#include <glm\gtc\matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float viewportWidth, float viewportHeight, float verticalFOV, float near, float far)
	: Camera(position, direction, viewportWidth, viewportHeight, near, far)
	, m_fieldOfView(verticalFOV)
{
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_viewportWidth / m_viewportHeight, m_near, m_far);
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
	m_projectionMatrix = glm::perspective(m_fieldOfView, m_viewportWidth / m_viewportHeight, m_near, m_far);
}