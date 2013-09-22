#include "Game.h"

Game::Game() {

}

Game::~Game()
{

}

void Game::render(float deltaSec) {
	if(m_screen != 0) {
		m_screen->render(deltaSec);
	}
}

void Game::resize(int width, int height) {
	if(m_screen != 0) {
		m_screen->resize(width, height);
	}
}
