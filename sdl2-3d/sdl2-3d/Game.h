#pragma once

#include "Engine/Input/Input.h"
#include "Engine/Graphics/Graphics.h"

#include <SDL.h>
#include <lua.hpp>
#include <LuaBridge.h>

class IScreen;

/** Manages static classes for easy access to global things (Input/Window etc.). Also contains the game loop. */
class Game
{
public:
	static void startLoop();
	static void stopLoop();


	static void resize(int width, int height);
	static void setScreen(IScreen* screen);
	inline static unsigned int getSDLTicks() { return SDL_GetTicks(); };

	/** Setup a lua state, loading the libaries and fixing the print() */
	static void initLua(lua_State* const L);
	/** Get the currently set lua_State */
	static lua_State* const L() { return s_L; };
	static void setLuaState(lua_State* L)
	{
		s_L = L;
	};

	static Input const input;
	static Graphics const graphics;

private:

	static void handleInput();
	static void update(float deltaSec);
	static void render(float deltaSec);
	static void updateFPSCounter(float deltaSec);

	static void startRenderThread();
	static void stopRenderThread();
	static int renderThreadLoop(void* ptr);

	static SDL_Thread* s_renderThread;

	static bool s_running;
	static IScreen* s_currScreen;
	static lua_State* s_L;

	Game() {};
	~Game() {};
	Game(const Game& copy) = delete;
};