#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "Screens\GameScreen.h"
#include "Engine\Model\ScreenManager.h"

#include <vector>

class IScreen;
class GameLoop : public ScreenManager 
{
public:
	GameLoop();
	~GameLoop();

	static void shutDown();
private:
	void initGL();
	IScreen* m_gameScreen;
	static bool m_running;
};

#endif //GAME_LOOP_H_