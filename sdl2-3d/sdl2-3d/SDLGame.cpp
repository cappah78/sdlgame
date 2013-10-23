#include "SDLGame.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Screen.h"
#include "KeyListener.h"
#include "MouseListener.h"

SDL_Window* SDLGame::mainWindow;
SDLGame* SDLGame::instance;
int SDLGame::screenWidth;
int SDLGame::screenHeight;

SDLGame::SDLGame(SDL_Window* window)
{
	instance = this;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	initGL();

	mainWindow = window;

	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	Uint32 startTime = SDL_GetTicks();
	Uint32 renderCount = 0;
	float timePassed = 0;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
				for (auto i = m_mouseListeners.begin(); i != m_mouseListeners.end(); i++)
					if ((*i)->mouseMoved(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel))
						break;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!mouseCaptured)
				{
					mouseCaptured = true;
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}

				for(auto i = m_mouseListeners.begin(); i != m_mouseListeners.end(); i++)
					if ((*i)->mouseDown(event.button.button, event.button.x, event.button.y))
						break;
				break;
			case SDL_MOUSEBUTTONUP:
				for(auto i = m_mouseListeners.begin(); i != m_mouseListeners.end(); i++)
					if ((*i)->mouseUp(event.button.button, event.button.x, event.button.y))
						break;
				break;
			case SDL_MOUSEWHEEL:
				for(auto i = m_mouseListeners.begin(); i != m_mouseListeners.end(); i++)
					if ((*i)->mouseScrolled(event.wheel.y))
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

				for(auto i = m_keyListeners.begin(); i != m_keyListeners.end(); i++)
					if ((*i)->keyDown(event.key.keysym))
						break;
				break;
			case SDL_KEYUP:
				for(auto i = m_keyListeners.begin(); i != m_keyListeners.end(); i++)
					if ((*i)->keyUp(event.key.keysym))
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
			itoa(renderCount, title, 10);
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

SDLGame::~SDLGame() 
{

}

void SDLGame::initGL()
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

void SDLGame::swap() 
{
	 SDL_GL_SwapWindow(mainWindow);
}

void SDLGame::registerKeyListener(KeyListener* listener)
{
	SDLGame::instance->m_keyListeners.push_back(listener);
}

void SDLGame::registerMouseListener(MouseListener* listener)
{
	SDLGame::instance->m_mouseListeners.push_back(listener);
}

void SDLGame::unregisterKeyListener(KeyListener* listener)
{
	std::vector<KeyListener*>* list = &SDLGame::instance->m_keyListeners;
	list->erase(std::remove(list->begin(), list->end(), listener), list->end());
}

void SDLGame::unregisterMouseListener(MouseListener* listener)
{
	std::vector<MouseListener*>* list = &SDLGame::instance->m_mouseListeners;
	list->erase(std::remove(list->begin(), list->end(), listener), list->end());
}

int SDLGame::getWidth() 
{
	return screenWidth;
}

int SDLGame::getHeight()
{
	return screenHeight;
}