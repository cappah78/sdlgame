#ifndef I_MOUSE_LISTENER_H_
#define I_MOUSE_LISTENER_H_

#include <SDL.h>

/** Can register to the Input class to receive mouse events */
class IMouseListener
{
public:
	virtual ~IMouseListener() {};
	/*
		Mouse buttons: 
			1 = LMB,
			2 = Mouse wheel click,
			3 = RMB,
			4+ additional mouse buttons.
	*/
	/** Called when a mouse button is pressed, return if the event was handled and should not be received by other listeners */
	virtual bool mouseDown(Uint8 key, int xPos, int yPos) { return false; }; 
	/** Called when a mouse button is released, return if the event was handled and should not be received by other listeners */
	virtual bool mouseUp(Uint8 key, int xPos, int yPos) { return false; };
	/** Called when the mouse is moved, return if the event was handled and should not be received by other listeners */
	virtual bool mouseMoved(int xPos, int yPos, int xMove, int yMove) { return false; };
	/** Called when the mousewheel is scrolled, return if the event was handled and should not be received by other listeners */
	virtual bool mouseScrolled(int amount) { return false; };
};


#endif // I_MOUSE_LISTENER_H_