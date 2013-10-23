#ifndef TILED_SHADER_POINT_LIGHT_H_
#define TILED_SHADER_POINT_LIGHT_H_

#include <glm/glm.hpp>
#include <vector>

struct TiledShaderPointLight
{
public:
	glm::vec3 position;
	glm::vec3 color;
	float range;
};


inline TiledShaderPointLight makePointLight(const glm::vec3 &position, const glm::vec3 &color, float range)
{
	TiledShaderPointLight l = { position, color, range };
	return l;
}

inline TiledShaderPointLight makePointLight(const glm::vec3 &position, const TiledShaderPointLight &l)
{
	return makePointLight(position, l.color, l.range);
}

typedef std::vector<TiledShaderPointLight> TiledShaderPointLights;

#endif // TILED_SHADER_POINT_LIGHT_H_
