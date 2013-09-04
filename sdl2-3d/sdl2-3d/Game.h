#ifndef GAME_H_
#define GAME_H_

#include "Screen.h"

class Game {
public:
	Game();
	~Game();
	virtual void render(float deltaSec);
	virtual void resize(int width, int height);

	void setScreen(Screen* screen_) 
	{
		screen = screen_;
	}

	const Screen *getScreen() 
	{
		return screen;
	}

private:
	Screen* screen;
};

#endif //GAME_H_