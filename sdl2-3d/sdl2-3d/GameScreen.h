#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

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

class Terrain;
class Texture;

class GameScreen : public Screen
{
public:
	GameScreen();
	~GameScreen();

	void render(float deltaSec);
	void resize(int width, int height);
private:
	int screenWidth;
	int screenHeight;

	coment::World world;
	entitysystem::MovementSystem movementSystem;
	entitysystem::MouseLookSystem mouseLookSystem;

	PerspectiveCamera camera;
	FPSCameraController cameraController;
	HeightMap heightMap;

	//Shader skyBoxShader;
	//SkyBox skyBox;
	Shader terrainShader;

	GBuffer gBuffer;
	LightManager lightManager;

	Texture* texture;
	Terrain* terrain;
};



#endif //GAMESCREEN_H_