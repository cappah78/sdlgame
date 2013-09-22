#include "LightSorter.h"

#include "Light.h"
#include "Camera.h"

LightSorter::LightSorter(const Camera& camera)
	: m_camera(camera)
{

}

LightSorter::~LightSorter()
{

}

bool LightSorter::operator() (const Light* light1, const Light* light2)
{
	float dist1 = glm::distance(m_camera.m_position, light1->m_position);
	float dist2 = glm::distance(m_camera.m_position, light2->m_position);

	return dist1 < dist2;
}