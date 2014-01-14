#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <glm\glm.hpp>

class Frustum
{
public:
	Frustum() {};
	~Frustum() {};
	Frustum(const Frustum& copy) = delete;

	void calculateFrustum(const glm::mat4& mvp);
	bool pointInFrustum(const glm::vec3& point) const;
	bool sphereInFrustum(const glm::vec3& point, float radius) const;

private:
	glm::vec4 planes[6];
};

#endif //FRUSTUM_H_