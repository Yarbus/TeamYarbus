/******************************************************************************/
/*!
\file   Health.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/
#pragma once
#include "Component.h"

namespace Framework
{
	class UpdateEvent;
	class CollisionEvent;
	class Health : public Component
	{
	public:
		const static std::string Name;
		Health(GameObject * obj)
		{
			gameObject = obj;
		}
		Health(){}

		~Health();
		void reFuel();
		void Update(UpdateEvent* e);
		void OnCollisionEnter(CollisionEvent* c);
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Initialize();

	private:
		float maxRadius;
		float minRadius;
		float currentRadius;
		bool alive;
		bool reFueling;
	};
}