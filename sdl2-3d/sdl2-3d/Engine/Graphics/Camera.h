#pragma once

#include <glm\glm.hpp>
#include "Frustum.h"

/** 
Based camera class, updates the projection-view matrix for rendering.
*/
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 direction, float viewportWidth, float viewportHeight, float near, float  far);
	virtual ~Camera();

	virtual void translate(float x, float y, float z);
	/** Translates the camera position relative to the view of the camera */
	virtual void translateRelative(float x, float y, float z);
	virtual void rotate(float angle, glm::vec3 axis);
	virtual void rotateRelative(float xRot, float yRot);
	virtual void lookAt(float x, float y, float z);
	virtual void lookAtDir(glm::vec3 dir);
	virtual void resize(int width, int height);
	virtual void update();
	virtual void setNearFar(float near, float far);

	/** True if any of the vertices are contained in the frustum */
	bool frustumContains(const glm::vec3* const vertices, unsigned int numVertices) const;
	/** True if any of the spheres are contained in the frustum */
	bool frustumContainsSpheres(const glm::vec3* const vertices, unsigned int numVertices, float sphereRadius) const;

	/** rotation on the x/y plane from -180 to 180, 0 == up */
	float getRotationRadXY();
	/** rotation on the x/z plane from -180 to 180, 0 == up */
	float getRotationRadXZ();
	/** rotation on the y/z plane from -180 to 180, 0 == up */
	float getRotationRadYZ();

	// public all the things because convenience
	glm::mat4 m_combinedMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;

	Frustum m_frustum;

	float m_viewportWidth;
	float m_viewportHeight;
	float m_near;
	float m_far;
};