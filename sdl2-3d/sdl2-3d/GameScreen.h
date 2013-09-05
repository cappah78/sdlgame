#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include "Screen.h"
#include "MouseListener.h"
#include "KeyListener.h"

#include "Texture.h"
#include "TextureRegion.h"
#include "Pixmap.h"
#include "HeightMap.h"
#include "Terrain2.h"
#include "Shader.h"
#include "LightManager.h"

#include "coment\World.h"
#include "EntityFactory.h"

#include "MovementSystem.h"
#include "MouseLookSystem.h"

#include "SpriteBatch.h"
#include "SDLGame.h"
#include "PerspectiveCamera.h"
#include "FPSCameraController.h"

class GameScreen : public Screen
{
public:
	GameScreen();
	~GameScreen();

	void render(float deltaSec);
	void resize(int width, int height);

private:
	coment::World world;
	entitysystem::MovementSystem movementSystem;
	entitysystem::MouseLookSystem mouseLookSystem;

	LightManager lightManager;
	Shader terrainShader;

	HeightMap heightMap;

	SpriteBatch batch;
	PerspectiveCamera camera;
	FPSCameraController cameraController;

	Texture* texture;
	Texture* texture2;
	Texture* texture3;
	TextureRegion* region;

	Terrain2* terrain;
};



#endif //GAMESCREEN_H_