#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include <gl\glew.h>

#include "IScreen.h"
#include "IMouseListener.h"
#include "IKeyListener.h"

#include "SkyBox.h"
#include "Material.h"

#include "SDLGame.h"
#include "PerspectiveCamera.h"
#include "FPSCameraController.h"

class Texture;
struct TextureRegion;
class Material;
class VoxelBatch;
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
	Material m_material;

	VoxelBatch* m_voxelBatch;
	VoxelCache* m_voxelCache;
	Texture* m_texture;
	TextureRegion* m_region;
};

#endif //GAME_SCREEN_H_