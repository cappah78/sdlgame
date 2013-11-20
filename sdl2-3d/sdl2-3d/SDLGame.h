#ifndef SDLGAME_H_
#define SDLGAME_H_

#include "GameScreen.h"

#include <vector>
#include "Game.h"

class IKeyListener;
class IMouseListener;
class IScreen;
struct SDL_Window;

class SDLGame : public Game 
{
public:
	//singleton
	static SDLGame* instance;

	SDLGame(SDL_Window* mainWindow);
	~SDLGame();

	static void registerKeyListener(IKeyListener* listener);
	static void registerMouseListener(IMouseListener* listener);

	static void unregisterKeyListener(IKeyListener* listener);
	static void unregisterMouseListener(IMouseListener* listener);

	static int getWidth();
	static int getHeight();
	static void swap();	/* Update the screen */
	static SDL_Window* SDLGame::mainWindow; /* Our window handle */

private:
	void initGL();

	static int screenWidth;
	static int screenHeight;

	IScreen* gameScreen;

	std::vector<IKeyListener*> m_keyListeners;
	std::vector<IMouseListener*> m_mouseListeners;
};

#endif //SDLGAME_H_