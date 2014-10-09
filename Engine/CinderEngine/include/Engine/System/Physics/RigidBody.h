/******************************************************************************/
/*!
\file   RigidBody.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Vec2.h"
#include "Component.h"
#include "Common.h"
#include "glm.hpp"

namespace Framework
{
	class RigidBody : public Component
	{
	public:
		// The non-base component uses DefineComponentName macro to name component

		enum DynamicState
		{
			Static,    //unmoving
			Kinematic, //moving position, velocity acceleration
			Dynamic    //moving with kinematic & all forces considered
		};

		const static std::string Name;
		bool allowSleep;
		bool rotationLocked;
		DynamicState state;
		glm::vec2 vel;
		glm::vec2 angVel;


		RigidBody(GameObject * obj)
		{
			gameObject = obj;
		};

		~RigidBody();

		float getMass(void)
		{
			return mass;
		}


	private:
		float mass;

	};
} //Framework