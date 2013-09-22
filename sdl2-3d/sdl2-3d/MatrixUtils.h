#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

#include <glm/glm.hpp>

class MatrixUtils
{
public:
	static void setToViewMatrix(glm::mat4& viewMatrix, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
private:
	MatrixUtils() {};
};

#endif MATRIX_UTILS_H_