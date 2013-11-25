#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H_

#include "IScreen.h"
#include "../../Game.h"

class ScreenManager {
public:
	ScreenManager() {};
	~ScreenManager() {};



private:
	static int m_width;
	static int m_height;
	static IScreen* m_screen;
};

#endif //SCREEN_MANAGER_H_