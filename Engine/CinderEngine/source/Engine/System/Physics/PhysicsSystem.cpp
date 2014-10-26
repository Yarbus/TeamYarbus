/******************************************************************************/
/*!
\file   PhysicsSystem.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "PhysicsSystem.h"
#include "ObjectSystem.h"

namespace Framework
{
	namespace Physics
	{
		bool PhysicsSystem::Initialize()
		{
			gravityDirection = { 0, -1 };
			std::cout << "PhysicsSystem Initialized." << std::endl;
			return true;
		}

		// Called every frame
		void PhysicsSystem::Update(const double dt)
		{
			for (auto i = OBJECTSYSTEM->GameObjects.begin(); i != OBJECTSYSTEM->GameObjects.end(); ++i)
			{
				for (auto j = i; j != OBJECTSYSTEM->GameObjects.end(); ++j)
				{
					if (i != j)
					{

					}
				}
				if (i->second->RigidBody)
				{
					if (i->second->RigidBody->state == RigidBody::Dynamic)
					{
						i->second->RigidBody->vel += gravityDirection * Constant::g;
					}
					i->second->Transform->Translate(i->second->RigidBody->vel.x * .016, i->second->RigidBody->vel.y * .016, 0);
				}
			}
			//  go1->CircleCollider->DetectLine (go3line->LineCollider);
			//  go1->CircleCollider->DetectCircle (go2->CircleCollider);
			//const float currentTime = GetCurrentTime();
			//accumulator += currentTime - frameStart;
			//frameStart = currentTime;
		
		}
		//{
		//	UpdateCollision();
		//	UpdateResolution();
		//	UpdateForces();
		//}
	}
}