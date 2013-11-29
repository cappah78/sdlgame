#ifndef GAME_H_
#define GAME_H_

#include "Engine/Input/Input.h"
#include "Engine/Graphics/Graphics.h"

#include <SDL.h>

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include <lua.hpp>
#include <LuaBridge.h>

class IScreen;

class Game
{
public:
	static Input const input;
	static Graphics const graphics;
	static lua_State* const L;

	static void startGameLoop();
	static void shutdownGameLoop();

	static void render(float deltaSec);
	static void resize(int width, int height);
	static void setScreen(IScreen* screen);

	static void initLua();

private:
	static bool m_running;
	static IScreen* m_currScreen;

	~Game();
	Game(const Game& copy);
};

#endif //GAME_H_