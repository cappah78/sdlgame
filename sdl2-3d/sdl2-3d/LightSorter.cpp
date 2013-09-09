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
	float dist1 = glm::distance(camera.position, light1->position);
	float dist2 = glm::distance(camera.position, light2->position);

	return dist1 < dist2;
}