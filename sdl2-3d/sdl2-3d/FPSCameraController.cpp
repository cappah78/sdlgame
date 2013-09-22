#include "FPSCameraController.h"

#include "PerspectiveCamera.h"
#include "coment\World.h"

const float m_cameraSPEED = 100.0f;

const float DIAGONAL_SPEED = sqrt(pow(m_cameraSPEED, 2) + pow(m_cameraSPEED, 2));

namespace entitysystem
{
	FPSCameraController::FPSCameraController(PerspectiveCamera& camera)
		: m_camera(camera)
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

		bool
			w = m_isWPressed,
			a = m_isAPressed,
			s = m_isSPressed,
			d = m_isDPressed,
			space = m_isSpacePressed,
			shift = m_isShiftPressed;

		if (w && s)
			w = false, s = false;
		if (a && d)
			a = false, d = false;
		if (space && shift)
			space = false, shift = false;


		if (w)
		{
			if (a)
			{
				m_camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			}
			else if (d)
			{
				m_camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			}
			else
			{
				m_camera.translateRelative(0.0f, 0.0f, -m_cameraSPEED * delta);
			}
		}
		else if (s)
		{
			if (a)
			{
				m_camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			}
			else if (d)
			{
				m_camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			}
			else
			{
				m_camera.translateRelative(0.0f, 0.0f, m_cameraSPEED * delta);
			}
		}
		else if (a)
		{
			m_camera.translateRelative(-m_cameraSPEED * delta, 0.0f, 0.0f);
		}
		else if (d)
		{
			m_camera.translateRelative(m_cameraSPEED * delta, 0.0f, 0.0f);
		}

		if (space)
		{
			m_camera.translateRelative(0.0f, m_cameraSPEED * delta, 0.0f);
		}
		else if (shift)
		{
			m_camera.translateRelative(0.0f, -m_cameraSPEED * delta, 0.0f);
		}
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
			m_camera.rotateRelative(xMove * 0.2f, yMove * 0.2f);
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