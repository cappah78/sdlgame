#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include <gl\glew.h>

#include "Screen.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "HeightMap.h"
#include "LightManager.h"
#include "SkyBox.h"
#include "coment\World.h"
#include "EntityFactory.h"
#include "MovementSystem.h"
#include "MouseLookSystem.h"
#include "SDLGame.h"
#include "PerspectiveCamera.h"
#include "FPSCameraController.h"
#include "ForwardShader.h"

class Terrain;
class Texture;

class GameScreen : public Screen, public KeyListener
{
public:
	GameScreen();
	~GameScreen();

	void render(float deltaSec);
	void resize(int width, int height);
	virtual bool keyDown(SDL_Keysym key) override;
	virtual bool keyUp(SDL_Keysym key) override;
private:
	coment::World m_world;

	entitysystem::MovementSystem m_movementSystem;
	entitysystem::MouseLookSystem m_mouseLookSystem;
	entitysystem::FPSCameraController m_cameraController;

	PerspectiveCamera m_camera;
	HeightMap m_heightMap;

	SkyBox m_skyBox;

	ForwardShader forwardShader;
	LightManager m_lightManager;

	Texture* m_texture;
	Terrain* m_terrain;
};

#endif //GAME_SCREEN_H_