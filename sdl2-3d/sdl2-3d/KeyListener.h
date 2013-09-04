#ifndef KEYLISTENER_H_
#define KEYLISTENER_H_

#include <SDL.h>

class KeyListener
{
public:
	virtual ~KeyListener() {};
	virtual bool keyDown(SDL_Keysym key) { return false; };	/* Called when a key is pressed, return if the event was handled and should not be received by other listeners */
	virtual bool keyUp(SDL_Keysym key) { return false; }; 	/* Called when a key is released, return if the event was handled and should not be received by other listeners */
};


#endif // KEYLISTENER_H_