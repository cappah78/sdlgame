#ifndef MOUSELOOKSYSTEM_H_
#define MOUSELOOKSYSTEM_H_

#include "DeltaProcessingSystem.h"
#include "MouseListener.h"

#include <SDL.h>
#include <glm\glm.hpp>

namespace entitysystem
{
	class MouseLookSystem : public DeltaProcessingSystem, public MouseListener
	{
	public:
		MouseLookSystem();

		void registerComponents();
		virtual void process(const coment::Entity& e);

		bool mouseDown(Uint8 key, int xPos, int yPos); 
		bool mouseUp(Uint8 key, int xPos, int yPos);
		bool mouseMoved(int xPos, int yPos, int xMove, int yMove);

		glm::vec3 getLookDir()
		{
			return lookDir;
		};

	private:
		bool
			lmbPressed,
			rmbPressed;
		glm::vec3 lookDir;
	};
}

#endif //MOUSELOOKSYSTEM_H_