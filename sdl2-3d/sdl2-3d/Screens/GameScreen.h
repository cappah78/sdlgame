#pragma once

#include "../Engine/Model/IScreen.h"
#include "../Engine/Model/IMouseListener.h"
#include "../Engine/Model/IKeyListener.h"

#include "../Engine/Graphics/PerspectiveCamera.h"
#include "../Engine/Utils/FPSCameraController.h"

/** Main screen containing executing the render and managing all entities */
class GameScreen : public IScreen, public IKeyListener
{
public:
	GameScreen();
	~GameScreen();
	void render(float deltaSec) override;
	void resize(int width, int height) override;
	virtual bool keyDown(SDL_Keysym key) override;
	virtual bool keyUp(SDL_Keysym key) override;

private:
	FPSCameraController m_cameraController;
	PerspectiveCamera m_camera;
};
