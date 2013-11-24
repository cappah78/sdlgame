#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "GameScreen.h"

#include <vector>
#include "Game.h"

class IKeyListener;
class IMouseListener;
class IScreen;
struct SDL_Window;
struct lua_State;

class GameLoop : public Game 
{
public:
	GameLoop(SDL_Window* mainWindow);
	~GameLoop();

	static void registerKeyListener(IKeyListener* listener);
	static void registerMouseListener(IMouseListener* listener);

	static void unregisterKeyListener(IKeyListener* listener);
	static void unregisterMouseListener(IMouseListener* listener);

	static int getWidth();
	static int getHeight();
	static void swap();	/* Update the screen */


private:
	void initGL();

	//singleton
	static GameLoop* instance;
	static SDL_Window* mainWindow;
	static int screenWidth;
	static int screenHeight;

	static lua_State* L;

	IScreen* gameScreen;

	std::vector<IKeyListener*> m_keyListeners;
	std::vector<IMouseListener*> m_mouseListeners;
};

#endif //GAME_LOOP_H_