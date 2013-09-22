#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include <gl\glew.h>

#include "Screen.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "HeightMap.h"
#include "Shader.h"
#include "LightManager.h"
#include "GBuffer.h"
#include "SkyBox.h"
#include "coment\World.h"
#include "EntityFactory.h"
#include "MovementSystem.h"
#include "MouseLookSystem.h"
#include "SpriteBatch.h"
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

	entitysystem::MovementSystem movementSystem_;
	entitysystem::MouseLookSystem mouseLookSystem_;
	entitysystem::FPSCameraController cameraController_;

	PerspectiveCamera m_camera;
	HeightMap heightMap_;

	SkyBox m_skyBox;

	ForwardShader forwardShader;
	LightManager m_lightManager;

	Texture* m_texture;
	Terrain* m_terrain;
};

#endif //GAME_SCREEN_H_