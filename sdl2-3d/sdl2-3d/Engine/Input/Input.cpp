#include "Input.h"

#include "../../Game.h"

#include "../Model/IMouseListener.h"
#include "../Model/IKeyListener.h"

#include <SDL.h>
#include <algorithm>

std::vector<IKeyListener*> Input::m_keyListeners;
std::vector<IMouseListener*> Input::m_mouseListeners;
bool Input::m_mouseCaptured;

void Input::registerKeyListener(IKeyListener* listener)
{
	m_keyListeners.push_back(listener);
}

void Input::registerMouseListener(IMouseListener* listener)
{
	m_mouseListeners.push_back(listener);
}

void Input::unregisterKeyListener(IKeyListener* listener)
{
	m_keyListeners.erase(std::remove(m_keyListeners.begin(), m_keyListeners.end(), listener), m_keyListeners.end());
}

void Input::unregisterMouseListener(IMouseListener* listener)
{
	m_mouseListeners.erase(std::remove(m_mouseListeners.begin(), m_mouseListeners.end(), listener), m_mouseListeners.end());
}

void Input::mouseMoved(int xPos, int yPos, int xMove, int yMove)
{
	for (IMouseListener* mouseListener : m_mouseListeners)
	if (mouseListener->mouseMoved(xPos, yPos, xMove, yMove))
		break;
}

void Input::mouseDown(unsigned char button, int xPos, int yPos)
{
	if (!m_mouseCaptured)
	{
		m_mouseCaptured = true;
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	for (IMouseListener* mouseListener : m_mouseListeners)
		if (mouseListener->mouseDown(button, xPos, yPos))
			break;
}

void Input::mouseUp(unsigned char button, int xPos, int yPos)
{
	for (IMouseListener* mouseListener : m_mouseListeners)
		if (mouseListener->mouseUp(button, xPos, yPos))
			break;
}

void Input::mouseScrolled(int amount)
{
	for (IMouseListener* mouseListener : m_mouseListeners)
		if (mouseListener->mouseScrolled(amount))
			break;
}

void Input::keyDown(SDL_Keysym& key)
{
	if (key.sym == SDLK_ESCAPE)
	{
		if (!m_mouseCaptured)
		{
			Game::shutdownGameLoop();
		}
		else
		{
			m_mouseCaptured = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

	for (IKeyListener* keyListener : m_keyListeners)
		if (keyListener->keyDown(key))
			break;
}

void Input::keyUp(SDL_Keysym& key)
{
	for (IKeyListener* keyListener : m_keyListeners)
		if (keyListener->keyUp(key))
			break;
}