#include "FPSCameraController.h"

#include "PerspectiveCamera.h"
#include "coment\World.h"

const float CAMERA_SPEED = 100.0f;

const float DIAGONAL_SPEED = sqrt(pow(CAMERA_SPEED, 2) + pow(CAMERA_SPEED, 2));

namespace entitysystem
{
	FPSCameraController::FPSCameraController(PerspectiveCamera& camera_)
		: camera(camera_)
		, lmbPressed(false)
		, rmbPressed(false)
		, isWPressed(false), isAPressed(false), isSPressed(false), isDPressed(false)
		, isSpacePressed(false), isShiftPressed(false)
	{

	}

	FPSCameraController::~FPSCameraController()
	{

	}

	void FPSCameraController::processEntities(std::vector<coment::Entity>& entities)
	{
		float delta = _world->getDelta();

		bool
			w = isWPressed,
			a = isAPressed,
			s = isSPressed,
			d = isDPressed,
			space = isSpacePressed,
			shift = isShiftPressed;

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
				camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			}
			else if (d)
			{
				camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, -DIAGONAL_SPEED * delta);
			}
			else
			{
				camera.translateRelative(0.0f, 0.0f, -CAMERA_SPEED * delta);
			}
		}
		else if (s)
		{
			if (a)
			{
				camera.translateRelative(-DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			}
			else if (d)
			{
				camera.translateRelative(DIAGONAL_SPEED * delta, 0.0f, DIAGONAL_SPEED * delta);
			}
			else
			{
				camera.translateRelative(0.0f, 0.0f, CAMERA_SPEED * delta);
			}
		}
		else if (a)
		{
			camera.translateRelative(-CAMERA_SPEED * delta, 0.0f, 0.0f);
		}
		else if (d)
		{
			camera.translateRelative(CAMERA_SPEED * delta, 0.0f, 0.0f);
		}

		if (space)
		{
			camera.translateRelative(0.0f, CAMERA_SPEED * delta, 0.0f);
		}
		else if (shift)
		{
			camera.translateRelative(0.0f, -CAMERA_SPEED * delta, 0.0f);
		}
	}

	bool FPSCameraController::mouseDown(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			lmbPressed = true;
		if (key == 3)
			rmbPressed = true;

		return false;
	}

	bool FPSCameraController::mouseUp(Uint8 key, int xPos, int yPos)
	{
		if (key == 1)
			lmbPressed = false;
		if (key == 3)
			rmbPressed = false;

		return false;
	}

	bool FPSCameraController::mouseMoved(int xPos, int yPos, int xMove, int yMove)
	{
		if (lmbPressed)
		{
			camera.rotateRelative(xMove * 0.2f, yMove * 0.2f);
		}

		return false;
	}

	bool FPSCameraController::keyDown(SDL_Keysym key)
	{
		switch (key.sym)
		{
		case SDLK_w:
			isWPressed = true;
			break;
		case SDLK_a:
			isAPressed = true;
			break;
		case SDLK_s:
			isSPressed = true;
			break;
		case SDLK_d:
			isDPressed = true;
			break;
		case SDLK_SPACE:
			isSpacePressed = true;
			break;
		case SDLK_LSHIFT:
			isShiftPressed = true;
			break;
		}

		return false;
	}

	bool FPSCameraController::keyUp(SDL_Keysym key)
	{
		switch (key.sym)
		{
		case SDLK_w:
			isWPressed = false;
			break;
		case SDLK_a:
			isAPressed = false;
			break;
		case SDLK_s:
			isSPressed = false;
			break;
		case SDLK_d:
			isDPressed = false;
			break;
		case SDLK_SPACE:
			isSpacePressed = false;
			break;
		case SDLK_LSHIFT:
			isShiftPressed = false;
			break;
		}

		return false;
	}

}