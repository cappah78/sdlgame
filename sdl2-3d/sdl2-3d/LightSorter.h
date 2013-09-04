#ifndef LIGHTSORTER_H_
#define LIGHTSORTER_H_

#include "Light.h"
#include "Camera.h"

class LightSorter
{
public:
	LightSorter(const Camera& camera);
	~LightSorter();

	bool operator() (const Light* light1, const Light* light2);
private:
	const Camera& camera;
};

#endif //LIGHTSORTER_H_