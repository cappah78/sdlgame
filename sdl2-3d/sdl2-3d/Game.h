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

	static void update(float deltaSec);
	static void render(float deltaSec);

	static void resize(int width, int height);
	static void setScreen(IScreen* screen);
	inline static unsigned int getSDLTicks() { return SDL_GetTicks(); };

	/** Setup a lua state, loading the libaries and fixing the print() */
	static void initLua(lua_State* const L);
	/** Get the currently set lua_State */
	static lua_State* const L() { return m_L; };
	static void setLuaState(lua_State* L)
	{
		m_L = L;
	};

	static Input const input;
	static Graphics const graphics;

private:

	static bool m_running;
	static IScreen* m_currScreen;
	static lua_State* m_L;

	Game() {};
	~Game() {};
	Game(const Game& copy) = delete;
};