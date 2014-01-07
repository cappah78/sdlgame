#ifndef I_KEY_LISTENER_H_
#define I_KEY_LISTENER_H_

#include <SDL.h>

/** Can register to the Input class to receive keyboard events */
class IKeyListener
{
public:
	virtual ~IKeyListener() {};
	virtual bool keyDown(SDL_Keysym key) { return false; };	/* Called when a key is pressed, return if the event was handled and should not be received by other listeners */
	virtual bool keyUp(SDL_Keysym key) { return false; }; 	/* Called when a key is released, return if the event was handled and should not be received by other listeners */
};


#endif // I_KEY_LISTENER_H_