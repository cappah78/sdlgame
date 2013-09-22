#include "MouseLookSystem.h"

#include "MouseFollow.h"
#include "Direction.h"
#include "MathUtils.h"
#include <glm\gtx\rotate_vector.hpp>

#define MOUSELOOK_SENSITIVITY 0.2f

static const glm::vec3 UP(0, 1, 0);

namespace entitysystem
{
	MouseLookSystem::MouseLookSystem(glm::vec3 lookDir)
		: m_lmbPressed(false)
		, m_rmbPressed(false)
		, m_lookDir(glm::normalize(lookDir))
	{

	}

	void MouseLookSystem::registerComponents()
	{
		registerComponent<component::MouseFollow>();
		registerComponent<component::Direction>();
	}

	void MouseLookSystem::process(const coment::Entity& e)
	{
		component::Direction* direction = _world->getComponent<component::Direction>(e);
		direction->dir = m_lookDir;
	}

	bool MouseLookSystem::mouseDown(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			m_lmbPressed = true;
		if (key == 3)
			m_rmbPressed = true;
		return false;
	}

	bool MouseLookSystem::mouseUp(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			m_lmbPressed = false;
		if (key == 3)
			m_rmbPressed = false;
		return false;
	}

	bool MouseLookSystem::mouseMoved(int xPos, int yPos, int xMove, int yMove)
	{
		if (m_lmbPressed)
		{
			//rotate horizontally
			m_lookDir = glm::rotate(m_lookDir, -xMove * MOUSELOOK_SENSITIVITY, UP);
			
			//calculate axis to rotate vertically on
			float xzAngle = math::atan2(m_lookDir.x, m_lookDir.z);
			glm::vec3 yRotAxis(-math::cos(xzAngle), 0.0f, math::sin(xzAngle));

			//rotate vertically
			glm::vec3 tmp = m_lookDir;
			m_lookDir = glm::rotate(m_lookDir, -yMove * MOUSELOOK_SENSITIVITY, yRotAxis);
			//limit vertical look movement

			if (m_lookDir.y > 0.99f || m_lookDir.y < -0.99f)
				m_lookDir = tmp;
		}
		return false;
	}
}