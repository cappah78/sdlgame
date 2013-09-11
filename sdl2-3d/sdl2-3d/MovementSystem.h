#ifndef MOVEMENTSYSTEM_H_
#define MOVEMENTSYSTEM_H_

#include "DeltaProcessingSystem.h"

namespace entitysystem
{
	class MovementSystem : public DeltaProcessingSystem
	{
	public:
		MovementSystem();
		void registerComponents();
		void process(const coment::Entity& e);
	};
}

#endif //MOVEMENTSYSTEM_H_