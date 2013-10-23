#ifndef FPSCAMERACONTROLLER_H_
#define FPSCAMERACONTROLLER_H_

#include "MouseListener.h"
#include "KeyListener.h"
#include "coment\systems\EntitySystem.h"
#include <glm\glm.hpp>

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
		PerspectiveCamera& m_camera;
		bool m_lmbPressed, m_rmbPressed;

		bool
			m_isWPressed,
			m_isAPressed,
			m_isSPressed,
			m_isDPressed,
			m_isSpacePressed,
			m_isShiftPressed;

		glm::vec3 m_lookDir;
	};
}
#endif //FPSCAMERACONTROLLER_H_