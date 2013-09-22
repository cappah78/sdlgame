#include "Camera.h"

#include <iostream>
#include <stdio.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "MathUtils.h"
#include "MatrixUtils.h"

static const glm::vec3 UP(0, 1, 0);

Camera::Camera(float viewportWidth, float viewportHeight)
	: m_position(0, 0, 0)
	, m_direction(1, 0, 0)
	, m_viewportWidth(viewportWidth)
	, m_viewportHeight(viewportHeight)
{

}

Camera::Camera(glm::vec3 position, glm::vec3 direction)
	: m_position(position)
	, m_direction(glm::normalize(direction))
	, m_up(UP)
{

}

Camera::~Camera()
{

}


void Camera::translate(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}

void Camera::translateRelative(float x, float y, float z)
{
	float angle = getRotationRadXZ();
	float xTrans = x * math::cos(angle) + z * math::sin(angle);
	float zTrans = z * math::cos(angle) - x * math::sin(angle);

	translate(-xTrans, y, -zTrans);
}

float Camera::getX()
{
	return m_position.x;
}

float Camera::getY()
{
	return m_position.y;
}

float Camera::getZ()
{
	return m_position.z;
}

void Camera::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

float Camera::getRotationRadXY()
{
	return math::atan2(m_direction.x, m_direction.y);
}

float Camera::getRotationRadXZ()
{
	return math::atan2(m_direction.x, m_direction.z);
}

float Camera::getRotationRadYZ()
{
	return math::atan2(m_direction.y, m_direction.z);
}

void Camera::rotate(float angleRad, glm::vec3 axis)
{
	glm::vec3 tmp = m_direction;
	m_direction = glm::rotate(m_direction, angleRad, axis);
	//limit vertical look movement
	if (m_direction.y > 0.99f || m_direction.y < -0.99f)
	{
		m_direction = tmp;
		return;
	}
	//recalculate up vector.
	glm::vec3 side = glm::cross(m_direction, UP);
	m_up = glm::cross(side, m_direction);
}

void Camera::rotateRelative(float xRot, float yRot)
{
	//rotate x component
	rotate(xRot, UP);

	//rotate y component
	float angle = getRotationRadXZ();
	rotate(yRot, glm::vec3(math::cos(angle), 0.0f, math::sin(angle)));
}

void Camera::update()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
	m_combinedMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::lookAt(float x, float y, float z)
{
	m_direction = glm::normalize(glm::vec3(x - m_position.x, y - m_position.y, z - m_position.z));
}

void Camera::lookAtDir(glm::vec3 direction)
{
	m_direction = direction;
}