#include "Game.h"

#include <SDL.h>
#include <SDL_thread.h>
#include <lua.hpp>

#include "Engine\Graphics\GL\GLCommandQueue.h"

#include "Engine\Utils\CheckGLError.h"

#include "Screens/GameScreen.h"

IScreen* Game::s_currScreen = NULL;
bool Game::s_running = true;
SDL_Thread* Game::s_renderThread = NULL;
bool Game::s_renderThreadRunning = true;

void Game::startLoop()
{
	startRenderThread();
	Uint32 startTime = SDL_GetTicks();
	
	while (s_running) 
	{
		Uint32 newTime = SDL_GetTicks();
		float deltaSec = (float) (newTime - startTime) / 1000.0f;
		startTime = newTime;

		handleInput();
		render(deltaSec);
		updateFPSCounter(deltaSec);
	}
}

GLCommandQueue s_glCommandQueue;

int Game::renderThreadLoop(void* ptr)
{
	SDL_GLContext context = Game::graphics.initializeGL();

	while (s_renderThreadRunning)
	{
		s_glCommandQueue.update();
	}

	Game::graphics.disposeGL(context);

	return 0;
}

void Game::startRenderThread()
{
	s_renderThread = SDL_CreateThread(renderThreadLoop, "Render Thread", (void*) NULL);
}

void Game::stopRenderThread()
{
	s_renderThreadRunning = false;
}

void Game::render(float deltaSec)
{
	if (s_currScreen != NULL)
		s_currScreen->render(deltaSec);
}

void Game::handleInput()
{
	static SDL_Event event;

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
			s_running = false;
			break;
		}
	}
}

void Game::updateFPSCounter(float deltaSec)
{
	static float timePassed = 0.0f;
	static int renderCount = 0;
	static char numFramesStr[10];

	timePassed += deltaSec;
	renderCount++;
	if (timePassed >= 1.0f)	//setting fps in title here
	{
		_itoa_s(renderCount, numFramesStr, 10);
		char str[16] = "FPS: ";
		strcat_s(str, 10, numFramesStr);
		Game::graphics.setWindowTitle(str);
		timePassed = 0.0;
		renderCount = 0;
	}
}

void Game::resize(int width, int height)
{
	graphics.resizeScreen(width, height);
	s_currScreen->resize(width, height);
}

void Game::setScreen(IScreen* screen)
{
	screen->resize(graphics.getScreenWidth(), graphics.getScreenHeight());
	s_currScreen = screen;
}

void Game::stopLoop()
{
	s_running = false;
	stopRenderThread();
}

int luaPrint(lua_State* L)
{
	int numArgs = lua_gettop(L);
	lua_getglobal(L, "tostring");

	for (int i = 0; i < numArgs; ++i)
	{
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i + 1);
		lua_call(L, 1, 1);
		const char* res = lua_tostring(L, -1);
		printf(res);
		if (i == numArgs - 1)
			printf("\n");
		else
			printf("\t");
		lua_pop(L, 1);
	}

	return 0;
}

void Game::initLua(lua_State* const L)
{
	luaL_openlibs(L);
	lua_register(L, "print", lua_CFunction(luaPrint));	// Fix broken print by overriding with a new function
}