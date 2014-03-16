#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../Game.h"
#include "../Engine/Graphics/GL/GLCommandQueue.h"
#include "../Engine/Utils/Functor.h"

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;
static const float CAMERA_FAR = 280.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 2, -10);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(0, 0, 1);

GLuint vao;
GLuint vertexBufferID = 5;
GLuint colorBufferID;

int printThing(const char* m, GLuint& da, int wa)
{
	printf(m, da);
	da++;
	return 2;
}

GameScreen::GameScreen()
	: m_camera(
		CAMERA_SPAWN_POS,
		CAMERA_SPAWN_DIR,
		float(Game::graphics.getScreenWidth()),
		float(Game::graphics.getScreenHeight()),
		CAMERA_VERTICAL_FOV,
		CAMERA_NEAR,
		CAMERA_FAR)
	, m_cameraController(m_camera, CAMERA_SPAWN_DIR)
	//, m_modelShader("Assets/Shaders/modelshader.vert", NULL, "Assets/Shaders/modelshader.frag")
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glqGenVertexArrays(1, &vao);
	glqGenBuffers(1, &vertexBufferID);
	glqGenBuffers(1, &colorBufferID);

	glqBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glqVertexAttribPointer(0, sizeof(glm::vec2), GL_FLOAT, false, 0, NULL);
	glqPrintGLuint(vertexBufferID);
	glqPrintGLuint(colorBufferID);

	glqBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	GLuint da = 1;;
	int out;
	Func::makeFunctor(out, printThing, "da", da, 5)->call();
	printf("da: %i \n", da);
}

void GameScreen::render(float deltaSec)
{
	glqClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glqClear(GL_COLOR_BUFFER_BIT);
	glqCheckGLError();
	glqSwapWindow();
}

GameScreen::~GameScreen() 
{

}

void GameScreen::resize(int width, int height) 
{
	glqViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

static int s_camSpeedTicks = 0;

bool GameScreen::keyDown(SDL_Keysym key)
{
	switch (key.sym)
	{
	case SDLK_r:
		//just some quick way to control speed
		s_camSpeedTicks++;
		m_cameraController.setCameraSpeed(10.0f * ((s_camSpeedTicks % 7) + 1));
		return true;
	case SDLK_PERIOD:
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far + 10.0f);
		return true;
	case SDLK_COMMA:
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far - 10.0f);
		return true;
	case SDLK_F1:
		printf("Controls: \n"
			"WASD, Shift, Space to fly \n"
			"R to toggle different speeds \n"
			"Period to increase render distance \n"
			"Comma to decrease render distance \n"
			"F to trigger a block update at location \n"
			"T to place a light at location(wip)\n");
		return true;
	default:
		return false;
	}
}

bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

