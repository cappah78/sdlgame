#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include "..\Engine\Model\IScreen.h"
#include "..\Engine\Model\IMouseListener.h"
#include "..\Engine\Model\IKeyListener.h"

#include "..\Engine\Graphics\Renderers\SkyBox.h"
#include "..\Engine\Graphics\PerspectiveCamera.h"
#include "..\Engine\Utils\FPSCameraController.h"

#include "..\Voxel\VoxelRenderer.h"
#include "../Voxel/VoxelWorld.h"
#include "../Voxel/WorldRenderer.h"
#include "../Voxel/Deferred/DeferredWorldRenderer.h"


class VoxelCache;
class TextureArray;

/** Main screen containing executing the render and managing all entities */
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
	//SkyBox m_skyBox;
	FPSCameraController m_cameraController;
	PerspectiveCamera m_camera;

	TextureManager m_textureManager;
	VoxelWorld m_world;
	WorldRenderer m_worldRenderer;
	DeferredWorldRenderer m_deferredWorldRenderer;
	VoxelRenderer m_voxelRenderer;
	TextureArray* m_tileSet;
};

#endif //GAME_SCREEN_H_