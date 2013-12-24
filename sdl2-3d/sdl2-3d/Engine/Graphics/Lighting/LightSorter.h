#ifndef LIGHT_SORTER_H_
#define LIGHT_SORTER_H_

//TODO: Rewrite lighting

class Camera;
class PointLight;
class SpotLight;

class SpotLightSorter
{
public:
	SpotLightSorter(const Camera& camera)
		: m_camera(camera)
	{};
	~SpotLightSorter() {};
	bool operator() (const SpotLight* light1, const SpotLight* light2);

private:
	const Camera& m_camera;
};

class PointLightSorter
{
public:
	PointLightSorter(const Camera& camera)
		: m_camera(camera) 
	{};
	~PointLightSorter() {};
	bool operator() (const PointLight* light1, const PointLight* light2);
private:
	const Camera& m_camera;
};

#endif //LIGHT_SORTER_H_
