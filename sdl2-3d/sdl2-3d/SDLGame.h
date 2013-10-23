#ifndef SDLGAME_H_
#define SDLGAME_H_

#include "GameScreen.h"

#include <vector>
#include "Game.h"

class KeyListener;
class MouseListener;
class Screen;
struct SDL_Window;

class SDLGame : public Game 
{
public:
	//singleton
	static SDLGame* instance;

	SDLGame(SDL_Window* mainWindow);
	~SDLGame();

	static void registerKeyListener(KeyListener* listener);
	static void registerMouseListener(MouseListener* listener);

	static void unregisterKeyListener(KeyListener* listener);
	static void unregisterMouseListener(MouseListener* listener);

	static int getWidth();
	static int getHeight();
	static void swap();	/* Update the screen */
	static SDL_Window* SDLGame::mainWindow; /* Our window handle */

private:
	void initGL();

	static int screenWidth;
	static int screenHeight;

	Screen* gameScreen;

	std::vector<KeyListener*> m_keyListeners;
	std::vector<MouseListener*> m_mouseListeners;
};

#endif //SDLGAME_H_