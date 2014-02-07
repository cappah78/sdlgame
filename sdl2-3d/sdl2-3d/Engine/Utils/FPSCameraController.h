#pragma once

#include "..\Model\IMouseListener.h"
#include "..\Model\IKeyListener.h"
#include <glm\glm.hpp>

class PerspectiveCamera;

/** 
Mouse/key listener which moves the given camera like a first person game would.
*/
class FPSCameraController : public IMouseListener, public IKeyListener
{
public:
	FPSCameraController(PerspectiveCamera& camera, const glm::vec3& startDir);
	~FPSCameraController();

	void update(float deltaSec);

	void setCameraSpeed(float cameraSpeed);

	bool keyDown(SDL_Keysym key);
	bool keyUp(SDL_Keysym key);
	bool mouseDown(Uint8 key, int xPos, int yPos);
	bool mouseUp(Uint8 key, int xPos, int yPos);
	bool mouseMoved(int xPos, int yPos, int xMove, int yMove);
private:
	PerspectiveCamera& m_camera;
	bool m_lmbPressed, m_rmbPressed;
	float m_cameraSpeed;
	float m_diagonalCameraSpeed;

	bool
		m_isWPressed,
		m_isAPressed,
		m_isSPressed,
		m_isDPressed,
		m_isSpacePressed,
		m_isShiftPressed;

	glm::vec3 m_lookDir;
};