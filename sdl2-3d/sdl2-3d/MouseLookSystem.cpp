#include "MouseLookSystem.h"

#define MOUSELOOK_SENSITIVITY 0.2f

static const glm::vec3 UP(0, 1, 0);

namespace entitysystem
{
	MouseLookSystem::MouseLookSystem()
		: lmbPressed(false)
		, rmbPressed(false)
		, lookDir(0, 0, -1)
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
		direction->dir = lookDir;
	}

	bool MouseLookSystem::mouseDown(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			lmbPressed = true;
		if (key == 3)
			rmbPressed = true;
		return false;
	}

	bool MouseLookSystem::mouseUp(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			lmbPressed = false;
		if (key == 3)
			rmbPressed = false;
		return false;
	}

	bool MouseLookSystem::mouseMoved(int xPos, int yPos, int xMove, int yMove)
	{
		if (lmbPressed)
		{
			//rotate horizontally
			lookDir = glm::rotate(lookDir, -xMove * MOUSELOOK_SENSITIVITY, UP);
			
			//calculate axis to rotate vertically on
			float xzAngle = math::atan2(lookDir.x, lookDir.z);
			glm::vec3 yRotAxis(-math::cos(xzAngle), 0.0f, math::sin(xzAngle));

			//rotate vertically
			glm::vec3 tmp = lookDir;
			lookDir = glm::rotate(lookDir, yMove * MOUSELOOK_SENSITIVITY, yRotAxis);
			//limit vertical look movement
			if (lookDir.y > 0.99f || lookDir.y < -0.99f)
				lookDir = tmp;
		}
		return false;
	}
}