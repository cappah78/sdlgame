#include "FPSCameraController.h"

#include "PerspectiveCamera.h"
#include "coment\World.h"
#include "MathUtils.h"
#include <glm\gtx\rotate_vector.hpp>

static const float CAMERA_SPEED = 100.0f;
static const float MOUSE_LOOK_SENSITIVITY = 0.2f;
static const float DIAGONAL_SPEED = sqrt(pow(CAMERA_SPEED, 2) + pow(CAMERA_SPEED, 2));

static const glm::vec3 UP(0, 1, 0);

namespace entitysystem
{
	FPSCameraController::FPSCameraController(PerspectiveCamera& camera)
		: m_camera(camera)
		, m_lookDir(1, 0, 0)
		, m_lmbPressed(false)
		, m_rmbPressed(false)
		, m_isWPressed(false), m_isAPressed(false), m_isSPressed(false), m_isDPressed(false)
		, m_isSpacePressed(false), m_isShiftPressed(false)
	{

	}

	FPSCameraController::~FPSCameraController()
	{

	}

	void FPSCameraController::processEntities(std::vector<coment::Entity>& entities)
	{
		float delta = _world->getDelta();

		bool w = m_isWPressed;
		bool a = m_isAPressed;
		bool s = m_isSPressed;
		bool d = m_isDPressed;
		bool space = m_isSpacePressed;
		bool shift = m_isShiftPressed;

		if (w && s)
			w = false, s = false;
		if (a && d)
			a = false, d = false;
		if (space && shift)
			space = false, shift = false;

		if (w)
		{
			if (a)
				m_camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			else if (d)
				m_camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			else
				m_camera.translateRelative(0.0f, 0.0f, -CAMERA_SPEED * delta);
		}
		else if (s)
		{
			if (a)
				m_camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			else if (d)
				m_camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			else
				m_camera.translateRelative(0.0f, 0.0f, CAMERA_SPEED * delta);
		}
		else if (a)
			m_camera.translateRelative(-CAMERA_SPEED * delta, 0.0f, 0.0f);
		else if (d)
			m_camera.translateRelative(CAMERA_SPEED * delta, 0.0f, 0.0f);

		if (space)
			m_camera.translateRelative(0.0f, CAMERA_SPEED * delta, 0.0f);
		else if (shift)
			m_camera.translateRelative(0.0f, -CAMERA_SPEED * delta, 0.0f);

		m_camera.lookAtDir(m_lookDir);
	}

	bool FPSCameraController::mouseDown(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			m_lmbPressed = true;
		if (key == 3)
			m_rmbPressed = true;
		return false;
	}

	bool FPSCameraController::mouseUp(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			m_lmbPressed = false;
		if (key == 3)
			m_rmbPressed = false;
		return false;
	}

	bool FPSCameraController::mouseMoved(int xPos, int yPos, int xMove, int yMove)
	{
		if (m_lmbPressed)
		{
			//rotate horizontally
			m_lookDir = glm::rotate(m_lookDir, -xMove * MOUSE_LOOK_SENSITIVITY, UP);

			//calculate axis to rotate vertically on
			float xzAngle = math::atan2(m_lookDir.x, m_lookDir.z);
			glm::vec3 yRotAxis(-math::cos(xzAngle), 0.0f, math::sin(xzAngle));

			//rotate vertically
			glm::vec3 tmp = m_lookDir;
			m_lookDir = glm::rotate(m_lookDir, -yMove * MOUSE_LOOK_SENSITIVITY, yRotAxis);
			//limit vertical look movement

			if (m_lookDir.y > 0.99f || m_lookDir.y < -0.99f)
				m_lookDir = tmp;
		}
		return false;
	}

	bool FPSCameraController::keyDown(SDL_Keysym key)
	{
		switch (key.sym)
		{
		case SDLK_w:
			m_isWPressed = true;
			break;
		case SDLK_a:
			m_isAPressed = true;
			break;
		case SDLK_s:
			m_isSPressed = true;
			break;
		case SDLK_d:
			m_isDPressed = true;
			break;
		case SDLK_SPACE:
			m_isSpacePressed = true;
			break;
		case SDLK_LSHIFT:
			m_isShiftPressed = true;
			break;
		}
		return false;
	}

	bool FPSCameraController::keyUp(SDL_Keysym key)
	{
		switch (key.sym)
		{
		case SDLK_w:
			m_isWPressed = false;
			break;
		case SDLK_a:
			m_isAPressed = false;
			break;
		case SDLK_s:
			m_isSPressed = false;
			break;
		case SDLK_d:
			m_isDPressed = false;
			break;
		case SDLK_SPACE:
			m_isSpacePressed = false;
			break;
		case SDLK_LSHIFT:
			m_isShiftPressed = false;
			break;
		}
		return false;
	}
}