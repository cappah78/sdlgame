#ifndef GAME_H_
#define GAME_H_

#include "IScreen.h"

class Game {
public:
	Game();
	~Game();
	virtual void render(float deltaSec);
	virtual void resize(int width, int height);

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