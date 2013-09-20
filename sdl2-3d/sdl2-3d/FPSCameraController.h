#ifndef FPSCAMERACONTROLLER_H_
#define FPSCAMERACONTROLLER_H_

#include "MouseListener.h"
#include "KeyListener.h"
#include "coment\systems\EntitySystem.h"

class PerspectiveCamera;

namespace entitysystem
{
	class FPSCameraController : public coment::EntitySystem, public MouseListener, public KeyListener
	{
	public:
		FPSCameraController(PerspectiveCamera& camera);
		~FPSCameraController();

		void registerComponents() {};
		virtual void processEntities(std::vector<coment::Entity>& entities);

		bool keyDown(SDL_Keysym key);
		bool keyUp(SDL_Keysym key);
		bool mouseDown(Uint8 key, int xPos, int yPos);
		bool mouseUp(Uint8 key, int xPos, int yPos);
		bool mouseMoved(int xPos, int yPos, int xMove, int yMove);
	private:
		PerspectiveCamera& camera;
		bool lmbPressed, rmbPressed;

		bool
			isWPressed,
			isAPressed,
			isSPressed,
			isDPressed,
			isSpacePressed,
			isShiftPressed;
	};
}
#endif //FPSCAMERACONTROLLER_H_