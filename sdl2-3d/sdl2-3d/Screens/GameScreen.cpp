#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../Game.h"
#include "../Engine/Graphics/GL/GLCommandQueue.h"
#include "../Engine/Utils/Functor.h"
#include "../Engine/Utils/ShaderManager.h"

#include <GL/glew.h>

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;
static const float CAMERA_FAR = 280.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 2, -10);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(0, 0, 1);

GLuint shaderID = 0;
GLuint vao = 0;
GLuint vertexBufferID = 0;
GLuint colorBufferID = 0;

GLboolean isBuffer;
GLboolean isVertexArray;

const char* vertShaderPath = "Assets/Shaders/simple.vert";
const char* geomShaderPath = NULL;
const char* fragShaderPath = "Assets/Shaders/simple.frag";

const GLfloat vertices[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

const GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};

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
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glqAddFunctor(Func::makeFunctor(shaderID, ShaderManager::createShaderProgram, vertShaderPath, geomShaderPath, fragShaderPath));

	glqGenVertexArrays(1, &vao);
	glqBindVertexArray(vao);

	glqGenBuffers(1, &vertexBufferID);
	glqBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glqEnableVertexAttribArray(0);
	glqVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
	glqBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void GameScreen::render(float deltaSec)
{
	glqClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glqClear(GL_COLOR_BUFFER_BIT);

	glqUseProgram(shaderID);
	glqBindVertexArray(vao);

	glqDrawArrays(GL_TRIANGLES, 0, 3);

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

