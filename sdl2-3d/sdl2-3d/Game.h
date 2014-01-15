#ifndef GAME_H_
#define GAME_H_

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
	static Input const input;
	static Graphics const graphics;

	/** Get the currently set lua_State */
	static lua_State* const L() { return m_L; };

	static void setLuaState(lua_State* L)
	{
		m_L = L;
	};

	static void startGameLoop();
	static void shutdownGameLoop();

	inline static unsigned int getSDLTicks() { return SDL_GetTicks(); };
	static void render(float deltaSec);
	static void resize(int width, int height);
	static void setScreen(IScreen* screen);

	/** Setup a lua state, loading the libaries and fixing the print() */
	static void initLua(lua_State* const L);

private:
	static bool m_running;
	static IScreen* m_currScreen;
	/** A global lua state which might be useful. */
	static lua_State* m_L;

	Game() {};
	~Game() {};
	Game(const Game& copy) = delete;
};

#endif //GAME_H_