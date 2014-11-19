/******************************************************************************/
/*!
\file   Resolution.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "IncludeForAllCollision.h"
#include "PhysicsLibrary.h"
#include "Resolution.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"

namespace Framework
{
	void ContactSet::Reset()
	{
		numOfContacts = 0;
	}

	void ContactSet::addContact(BodyContact * contact)
	{
		contactArray[numOfContacts++] = *contact;
	}

	void ContactSet::ResolveContacts(const float dt)
	{
		ResolvePositions(dt);
		ResolveVelocities(dt);
	}

	void ResolvePenetration(BodyContact& c, const float dt)
	{
		float invMassA = 0.0f;
		float invMassB = 0.0f;
		glm::vec3 velA = { 0, 0, 0 };
		glm::vec3 velB = { 0, 0, 0 };
		if (c.Bodies[0]->RigidBody)
		{
			invMassA = c.Bodies[0]->RigidBody->getInvMass();
			velA = c.Bodies[0]->RigidBody->vel;
		}
		if (c.Bodies[1]->RigidBody)
		{
			invMassB = c.Bodies[1]->RigidBody->getInvMass();
			velB = c.Bodies[1]->RigidBody->vel;
		}
		float totalInverseMass = invMassA + invMassB;

		glm::vec3 movePerImass = c.ContactNormal * (c.Penetration / totalInverseMass);

		movePerImass *= Physics::PHYSICSSYSTEM->penetrationResolvePercent;

		//calculate movement amounts
		c.Movement[0] = movePerImass * invMassA;
		c.Movement[1] = movePerImass * -invMassB;

		//apply penetration resolution
		Transform * moveValue0 = c.Bodies[0]->Transform;
		Transform * moveValue1 = c.Bodies[1]->Transform;
		c.Bodies[0]->Transform->Translate(velA);
		c.Bodies[1]->Transform->Translate(velB);
	}

	void ContactSet::ResolvePositions(const float dt)
	{
		int iterations = 0;
		int maxIterations = numOfContacts * 5;
		const float positionEpsilon = Physics::PHYSICSSYSTEM->penetrationEpsilon;

		while (iterations < maxIterations)
		{
			//find biggest penetration greater than 
			//correction epsilon
			float maxPenetration = positionEpsilon;
			int contactIndex = numOfContacts;

			for (int i = 0; i < numOfContacts; ++i)
			{
				if (contactArray[i].Penetration > maxPenetration)
				{
					maxPenetration = contactArray[i].Penetration;
					contactIndex = i;
				}
			}
			if (contactIndex == numOfContacts)
				break;

			//Resolve penetration - poke with stick method
			ResolvePenetration(contactArray[contactIndex], dt);

			//Update penetrations for related contacts
			glm::vec3* movement = contactArray[contactIndex].Movement;
			for (int i = 0; i < numOfContacts; ++i)
			{
				if (contactArray[i].Bodies[0] == contactArray[contactIndex].Bodies[0])
				{
					contactArray[i].Penetration -= glm::dot(movement[0], contactArray[i].ContactNormal);
				}
				else if (contactArray[i].Bodies[0] == contactArray[contactIndex].Bodies[1])
				{
					contactArray[i].Penetration -= glm::dot(movement[1], contactArray[i].ContactNormal);
				}
				if (contactArray[i].Bodies[1] == contactArray[contactIndex].Bodies[0])
				{
					contactArray[i].Penetration += glm::dot(movement[0], contactArray[i].ContactNormal);
				}
				else if (contactArray[i].Bodies[1] == contactArray[contactIndex].Bodies[0])
				{
					contactArray[i].Penetration += glm::dot(movement[1], contactArray[i].ContactNormal);
				}
			}
			++iterations;
		}
	}

	void ContactSet::ResolveVelocities(const float dt)
	{
		for (int i = 0; i < numOfContacts; ++i)
		{
			float imassA;
			glm::vec3 velA;
			if (contactArray[i].Bodies[0]->RigidBody)
			{
				imassA = contactArray[i].Bodies[0]->RigidBody->getInvMass();
				velA = contactArray[i].Bodies[0]->RigidBody->vel;
			}
			else
			{
				imassA = 0.0f;
				velA = { 0.0f, 0.0f, 0.0f };
			}
			float imassB;
			glm::vec3 velB;
			if (contactArray[i].Bodies[1] && contactArray[i].Bodies[1]->RigidBody)
			{
				imassB = contactArray[i].Bodies[1]->RigidBody->getInvMass();
				velB = contactArray[i].Bodies[1]->RigidBody->vel;
			}
			else
			{
				imassB = 0.0f;
				velB = { 0.0f, 0.0f, 0.0f };
			}

			float bunch11 = (imassB - imassA) / (imassA + imassB);
			float bunch12 = (2 * imassA) / (imassA + imassB);
			if (contactArray[i].Bodies[0]->RigidBody)
			{
				contactArray[i].Bodies[0]->RigidBody->vel.x = bunch11 * velA.x + bunch12 * velB.x;
				contactArray[i].Bodies[0]->RigidBody->vel.y = bunch11 * velA.y + bunch12 * velB.y;
				contactArray[i].Bodies[0]->RigidBody->vel = Physics::getReflection(contactArray[i].ContactNormal, -contactArray[i].Bodies[0]->RigidBody->vel);
			}

			float bunch21 = (imassB - imassA) / (imassA + imassB);
			float bunch22 = (2 * imassB) / (imassA + imassB);
			if (contactArray[i].Bodies[1]->RigidBody)
			{
				contactArray[i].Bodies[1]->RigidBody->vel.x = bunch21 * velA.x + bunch22 * velB.x;
				contactArray[i].Bodies[1]->RigidBody->vel.y = bunch21 * velA.y + bunch22 * velB.y;
				contactArray[i].Bodies[1]->RigidBody->vel = Physics::getReflection(contactArray[i].ContactNormal, -contactArray[i].Bodies[1]->RigidBody->vel);

			}
		}
	}
} //Framework