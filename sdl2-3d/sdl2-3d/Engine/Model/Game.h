#ifndef GAME_H_
#define GAME_H_

#include "IScreen.h"

class Game {
public:
	Game() : m_screen(0) {};
	~Game() {};

	virtual void Game::render(float deltaSec) 
	{
		if (m_screen != 0)
			m_screen->render(deltaSec);
	}

	virtual void Game::resize(int width, int height) 
	{
		if (m_screen != 0)
			m_screen->resize(width, height);
	}

	void setScreen(IScreen* screen)
	{
		m_screen = screen;
	}

	const IScreen *getScreen()
	{
		return m_screen;
	}

private:
	IScreen* m_screen;
};

#endif //GAME_H_