#include "Game.h"

#include <SDL.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include "Screens/GameScreen.h"
#include "Engine\Utils\CheckLuaError.h"

IScreen* Game::m_currScreen = NULL;
bool Game::m_running = true;

void Game::startGameLoop()
{
	Uint32 startTime = SDL_GetTicks();
	Uint32 renderCount = 0;
	float timePassed = 0;
	char title[20];

	SDL_Event event;

	while (m_running) {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					Game::resize(event.window.data1, event.window.data2);
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				Game::input.mouseMoved(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				break;
			case SDL_MOUSEBUTTONDOWN:
				Game::input.mouseDown(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				Game::input.mouseUp(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_MOUSEWHEEL:
				Game::input.mouseScrolled(event.wheel.y);
				break;
			case SDL_KEYDOWN:
				Game::input.keyDown(event.key.keysym);
				break;
			case SDL_KEYUP:
				Game::input.keyUp(event.key.keysym);
				break;
			case SDL_QUIT:
				m_running = false;
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
			_itoa_s(renderCount, title, 10);
			char str[20] = "FPS: ";
			strcat_s(str, 10, title);
			Game::graphics.setWindowTitle(str);
			timePassed = 0.0;
			renderCount = 0;
		}

		render(deltaSec);
	}
}

void Game::render(float deltaSec)
{
	if (m_currScreen != NULL)
		m_currScreen->render(deltaSec);
}

void Game::resize(int width, int height)
{
	graphics.resizeScreen(width, height);
	m_currScreen->resize(width, height);
}

void Game::setScreen(IScreen* screen)
{
	screen->resize(graphics.getScreenWidth(), graphics.getScreenHeight());
	m_currScreen = screen;
}

void Game::shutdownGameLoop()
{
	m_running = false;
}

int luaPrint(lua_State* L)
{
	int numArgs = lua_gettop(L);
	lua_getglobal(L, "tostring");

	for (int i = 0; i < numArgs; ++i)
	{
		lua_pushvalue(L, -1);		//TODO: verify that this is correct, just guessed
		lua_pushvalue(L, i + 1);
		lua_call(L, 1, 1);
		const char* res = lua_tostring(L, -1);
		std::cout << res;
		if (i == numArgs - 1)
			std::cout << std::endl;
		else
			std::cout << "\t";
		lua_pop(L, 1);
	}

	return 0;
}

void Game::initLua(lua_State* const L)
{
	luaL_openlibs(L);
	lua_register(L, "print", lua_CFunction(luaPrint));	// Fix broken print by overriding with a new function
}