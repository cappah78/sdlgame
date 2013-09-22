#ifndef GAME_H_
#define GAME_H_

#include "Screen.h"

class Game {
public:
	Game();
	~Game();
	virtual void render(float deltaSec);
	virtual void resize(int width, int height);

	void setScreen(Screen* screen) 
	{
		m_screen = screen;
	}

	const Screen *getScreen() 
	{
		return m_screen;
	}

private:
	Screen* m_screen;
};

#endif //GAME_H_