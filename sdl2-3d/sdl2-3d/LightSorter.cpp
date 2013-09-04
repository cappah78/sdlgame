#include "LightSorter.h"

LightSorter::LightSorter(const Camera& camera_)
	: camera(camera_)
{

}

LightSorter::~LightSorter()
{

}

bool LightSorter::operator() (const Light* light1, const Light* light2)
{
	float distance1 = glm::distance(camera.position, light1->position);
	float distance2 = glm::distance(camera.position, light2->position);

	return distance1 < distance2;
}