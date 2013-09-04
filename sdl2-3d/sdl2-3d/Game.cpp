#include "Game.h"

Game::Game() {

}

Game::~Game()
{

}

void Game::render(float deltaSec) {
	if(screen != 0) {
		screen->render(deltaSec);
	}
}

void Game::resize(int width, int height) {
	if(screen != 0) {
		screen->resize(width, height);
	}
}
