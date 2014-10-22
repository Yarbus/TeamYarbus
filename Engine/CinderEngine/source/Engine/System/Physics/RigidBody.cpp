/******************************************************************************/
/*!
\file   RigidBody.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "RigidBody.h"
#include "ComponentInclude.h"
#include "GameObject.h"

namespace Framework
{

	//Destructor
	RigidBody::~RigidBody()
	{
	}

  void RigidBody::Initialize ()
  {
    gameObject->RigidBody = this;
  }

  void RigidBody::Serialize (Serializer::DataNode* data)
  {

  }

	DefineComponentName(RigidBody);
}
