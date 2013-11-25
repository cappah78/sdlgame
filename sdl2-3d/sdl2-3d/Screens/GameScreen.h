#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include "..\Engine\Model\IScreen.h"
#include "..\Engine\Model\IMouseListener.h"
#include "..\Engine\Model\IKeyListener.h"

#include "..\Engine\Graphics\Renderers\SkyBox.h"
#include "..\Engine\Graphics\PerspectiveCamera.h"
#include "..\Engine\Utils\FPSCameraController.h"

class VoxelCache;

class GameScreen : public IScreen, public IKeyListener
{
public:
	GameScreen();
	~GameScreen();
	void render(float deltaSec);
	void resize(int width, int height);
	virtual bool keyDown(SDL_Keysym key) override;
	virtual bool keyUp(SDL_Keysym key) override;
private:
	SkyBox m_skyBox;
	FPSCameraController m_cameraController;
	PerspectiveCamera m_camera;

	VoxelCache* m_voxelCache;
};

#endif //GAME_SCREEN_H_