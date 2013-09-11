#include "Camera.h"

#include <iostream>
#include <stdio.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "MathUtils.h"

static const glm::vec3 UP(0, 1, 0);

Camera::Camera(glm::vec3 position_, glm::vec3 direction_)
	: position(position_)
	, direction(glm::normalize(direction_))
	, up(UP)
{

}

Camera::~Camera()
{

}

void Camera::translate(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Camera::translateRelative(float x, float y, float z)
{
	float angle = getRotationRadXZ();
	float xTrans = x * math::cos(angle) + z * math::sin(angle);
	float zTrans = z * math::cos(angle) - x * math::sin(angle);

	translate(xTrans, y, zTrans);
}

float Camera::getX()
{
	return position.x;
}

float Camera::getY()
{
	return position.y;
}

float Camera::getZ()
{
	return position.z;
}

void Camera::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

float Camera::getRotationRadXY()
{
	return math::atan2(direction.x, direction.y);
}

float Camera::getRotationRadXZ()
{
	return math::atan2(direction.x, direction.z);
}

float Camera::getRotationRadYZ()
{
	return math::atan2(direction.y, direction.z);
}

void Camera::rotate(float angleRad, glm::vec3 axis)
{
	glm::vec3 tmp = direction;
	direction = glm::rotate(direction, angleRad, axis);
	//limit vertical look movement
	if (direction.y > 0.99f || direction.y < -0.99f)
	{
		direction = tmp;
		return;
	}
	//recalculate up vector.
	glm::vec3 side = glm::cross(direction, UP);
	up = glm::cross(side, direction);
}

void Camera::rotateRelative(float xRot, float yRot)
{
	//rotate x component
	rotate(-xRot, UP);

	//rotate y component
	float angle = getRotationRadXZ();
	rotate(yRot, glm::vec3(-math::cos(angle), 0.0f, math::sin(angle)));
}

void Camera::update()
{
	viewMatrix = glm::lookAt(
		position,
		position - direction,
		up
	);
	combinedMatrix = projectionMatrix * viewMatrix;
}

void Camera::lookAt(float x, float y, float z)
{
	direction = glm::normalize(glm::vec3(x - position.x, y - position.y, z - position.z));
}

void Camera::lookAtDir(glm::vec3 dir)
{
	direction = dir;
}