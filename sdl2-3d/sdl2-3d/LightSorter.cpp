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
	return false;
}