#include "GameScreen.h"

#include "../Game.h"

#include "../Engine/Graphics/GL/GLCommandQueue.h"
#include "../Engine/Graphics/GL/GLVertexBuffer.h"
#include "../Engine/Graphics/GL/GLStateBuffer.h"
#include "../Engine/Graphics/GL/GLShader.h"

GLShader* shader;
GLStateBuffer* stateBuffer;
GLVertexBuffer* positionBuffer;
GLVertexBuffer* colorBuffer;

const GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};

const GLfloat positions[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

GameScreen::GameScreen()
{
	Game::input.registerKeyListener(this);

	shader = new GLShader("Assets/Shaders/simple.vert", NULL, "Assets/Shaders/simple.frag");

	stateBuffer = new GLStateBuffer();
	stateBuffer->enable();

	positionBuffer = new GLVertexBuffer(sizeof(positions), positions);
	positionBuffer->setAttribPointer(0, GL_FLOAT, 3);

	colorBuffer = new GLVertexBuffer(sizeof(colors), colors);
	colorBuffer->setAttribPointer(1, GL_FLOAT, 3);

	stateBuffer->disable();
}

void GameScreen::render(float deltaSec)
{
	glqClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glqClear(GL_COLOR_BUFFER_BIT);

	shader->begin();

	stateBuffer->enable();
	glqDrawArrays(GL_TRIANGLES, 0, 3);
	stateBuffer->disable();

	shader->end();

	glqSwapWindow();
}

GameScreen::~GameScreen() 
{
	delete colorBuffer;
	delete po
}

void GameScreen::resize(int width, int height) 
{
	glqViewport(0, 0, width, height);
}

bool GameScreen::keyDown(SDL_Keysym key)
{
	switch (key.sym)
	{
	case SDLK_w:
		return true;
	case SDLK_a:
		return true;
	case SDLK_s:
		return true;
	case SDLK_d:
		return true;
	default:
		return false;
	}
}

bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}


/*

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

*/