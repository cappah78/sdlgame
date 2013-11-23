#include "GameLoop.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "IScreen.h"
#include "IKeyListener.h"
#include "IMouseListener.h"

SDL_Window* GameLoop::mainWindow;
GameLoop* GameLoop::instance;
int GameLoop::screenWidth;
int GameLoop::screenHeight;

GameLoop::GameLoop(SDL_Window* window)
{
	instance = this;
	mainWindow = window;

	SDL_SetRelativeMouseMode(SDL_TRUE);
	initGL();
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glColorMask(false, false, false, false);
	Uint32 startTime = SDL_GetTicks();
	Uint32 renderCount = 0;
	float timePassed = 0;

	gameScreen = new GameScreen();
	gameScreen->resize(screenWidth, screenHeight);
	setScreen(gameScreen);

	SDL_Event event;

	bool mouseCaptured = true;

	char title[20];

	while (true) {
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_MOUSEMOTION:
				for (IMouseListener* mouseListener : m_mouseListeners)
					if (mouseListener->mouseMoved(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel))
						break;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!mouseCaptured)
				{
					mouseCaptured = true;
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
				for (IMouseListener* mouseListener: m_mouseListeners)
					if (mouseListener->mouseDown(event.button.button, event.button.x, event.button.y))
						break;
				break;
			case SDL_MOUSEBUTTONUP:
				for (IMouseListener* mouseListener : m_mouseListeners)
					if (mouseListener->mouseUp(event.button.button, event.button.x, event.button.y))
						break;
				break;
			case SDL_MOUSEWHEEL:
				for (IMouseListener* mouseListener : m_mouseListeners)
					if (mouseListener->mouseScrolled(event.wheel.y))
						break;

			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					if (mouseCaptured)
					{
						mouseCaptured = false;
						SDL_SetRelativeMouseMode(SDL_FALSE);
					}
					else goto exit_loop;
				}

				for (IKeyListener* keyListener : m_keyListeners)
					if (keyListener->keyDown(event.key.keysym))
						break;
				break;
			case SDL_KEYUP:
				for (IKeyListener* keyListener : m_keyListeners)
					if (keyListener->keyUp(event.key.keysym))
						break;
				break;

			case SDL_QUIT:
				goto exit_loop;
				break;
			}
		}

		Uint32 newTime = SDL_GetTicks();
		float deltaSec = (float) (newTime - startTime) / 1000.0f;
		startTime = newTime;

		timePassed += deltaSec;
		renderCount++;
		if (timePassed >= 1.0f)
		{
			_itoa(renderCount, title, 10);
			char str[20] = "FPS: ";
			std::strcat(str, title);
			SDL_SetWindowTitle(mainWindow, str);
			timePassed = 0.0;
			renderCount = 0;
		}

		Game::render(deltaSec);
	}
	exit_loop: ;	//goto

	delete gameScreen;
}

GameLoop::~GameLoop()
{

}

void GameLoop::initGL()
{
    glewExperimental=GL_TRUE;
    GLenum err = glewInit(); 

    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void GameLoop::swap()
{
	 SDL_GL_SwapWindow(mainWindow);
}

void GameLoop::registerKeyListener(IKeyListener* listener)
{
	GameLoop::instance->m_keyListeners.push_back(listener);
}

void GameLoop::registerMouseListener(IMouseListener* listener)
{
	GameLoop::instance->m_mouseListeners.push_back(listener);
}

void GameLoop::unregisterKeyListener(IKeyListener* listener)
{
	std::vector<IKeyListener*>& list = GameLoop::instance->m_keyListeners;
	list.erase(std::remove(list.begin(), list.end(), listener), list.end());
}

void GameLoop::unregisterMouseListener(IMouseListener* listener)
{
	std::vector<IMouseListener*>& list = GameLoop::instance->m_mouseListeners;
	list.erase(std::remove(list.begin(), list.end(), listener), list.end());
}

int GameLoop::getWidth()
{
	return screenWidth;
}

int GameLoop::getHeight()
{
	return screenHeight;
}