#include "MatrixUtils.h"
#include <iostream>
#include <stdio.h>

void MatrixUtils::setToViewMatrix(glm::mat4& viewMatrix, 
	const glm::vec3& position,
	const glm::vec3& direction,
	const glm::vec3& up)
{
	glm::vec3 f = glm::normalize(direction);
	glm::vec3 u = glm::normalize(up);
	glm::vec3 s = glm::normalize(glm::cross(f, u));
	u = glm::cross(s, f);

	std::cout << "s: " << u.x << ":" << u.y << ":" << u.z << std::endl;
	viewMatrix[0][0] = s.x;
	viewMatrix[1][0] = s.y;
	viewMatrix[2][0] = s.z;
	viewMatrix[0][1] = u.x;
	viewMatrix[1][1] = u.y;
	viewMatrix[2][1] = u.z;
	viewMatrix[0][2] = -f.x;
	viewMatrix[1][2] = -f.y;
	viewMatrix[2][2] = -f.z;
	viewMatrix[3] = viewMatrix[0] * -position[0] + viewMatrix[1] * -position[1] + viewMatrix[2] * -position[2];
	viewMatrix[3][3] = 1;
}