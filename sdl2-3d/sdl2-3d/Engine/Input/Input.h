#pragma once

#include <vector>

class IKeyListener;
class IMouseListener;
class Game;

struct SDL_Keysym;
#include <SDL_keycode.h>

/** 
Manages input related data, listeners can register to this and 
receive notifications of input.
*/
class Input
{
	friend class Game;
public:
	static void registerKeyListener(IKeyListener* listener);
	static void registerMouseListener(IMouseListener* listener);
	static void unregisterKeyListener(IKeyListener* listener);
	static void unregisterMouseListener(IMouseListener* listener);

	static void mouseMoved(int xPos, int yPos, int xMove, int yMove);
	static void mouseDown(unsigned char button, int xPos, int yPos);
	static void mouseUp(unsigned char button, int xPos, int yPos);
	static void mouseScrolled(int amount);

	static void keyDown(SDL_Keysym& key);
	static void keyUp(SDL_Keysym& key);

	static bool isKeyPressed(const SDL_Scancode& key);

private:
	Input() {};
	Input(const Input& copy) {};
	~Input() {};

	static std::vector<IKeyListener*> m_keyListeners;
	static std::vector<IMouseListener*> m_mouseListeners;
	static bool m_mouseCaptured;
};
