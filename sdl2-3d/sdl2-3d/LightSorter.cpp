#include "LightSorter.h"

#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"

bool PointLightSorter::operator() (const PointLight* light1, const PointLight* light2)
{
	float dist1 = glm::distance(m_camera.m_position, light1->m_position);
	float dist2 = glm::distance(m_camera.m_position, light2->m_position);

	return dist1 < dist2;
}

bool SpotLightSorter::operator() (const SpotLight* light1, const SpotLight* light2)
{
	float dist1 = glm::distance(m_camera.m_position, light1->m_position);
	float dist2 = glm::distance(m_camera.m_position, light2->m_position);

	return dist1 < dist2;
}