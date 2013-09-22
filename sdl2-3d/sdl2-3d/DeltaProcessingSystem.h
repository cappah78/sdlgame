#ifndef DELTAPROCESSINGSYSTEM_H_
#define DELTAPROCESSINGSYSTEM_H_

#include "coment\systems\EntitySystem.h"
#include "coment\World.h"
#include "coment\Entity.h"

namespace entitysystem 
{
	class DeltaProcessingSystem : public coment::EntitySystem
	{
	public:
		DeltaProcessingSystem() {};
		virtual ~DeltaProcessingSystem() {};

		virtual void registerComponents() = 0;
		virtual void process(const coment::Entity& e) = 0;
		// Process only if the system is enabled by default
		bool checkProcessing()
		{
			return getEnabled();
		};

		// Automatically call process for each entity
		void processEntities(std::vector<coment::Entity>& entities);

	protected:
		float m_delta;
	};
};

#endif //DELTAPROCESSINGSYSTEM_H_