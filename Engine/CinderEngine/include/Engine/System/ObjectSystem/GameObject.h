/******************************************************************************/
/*!
\file   Template.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.

\brief  Game Object make up everything in the Cinder Engine.
They are the core of every object and hold a Vector of pointers to all of
their components. A component is a part of an object (ie. Physics, graphic,
AI, controlers). Game Object have three different states which
they should be in during their lifespan.

-Constructed: The Game Object has been created and there are components
which are being added to it.  Data on the components is being added via
serialization or hard code.

-Intialization: After all data have been added, the composition is
ready to be used. If the game is running the systems will then
access each aplicable component when running their update function.

-R.I.P. : Composition is about to die and will be deleted by the
factory in the next loop.

*/
/******************************************************************************/


#pragma once


#define ComponentPointer(Name) Name * Name = nullptr

#include "ObjectSystem.h"
#include "Component.h"
#include <map>
#include "Vec2.h"
#include "IncludeForAllCollision.h"
#include "CharacterController.h"

namespace Framework
{
  class Component;
  
  class GameObject
  {

  public:
    ZilchDeclareBaseType(GameObject, Zilch::TypeCopyMode::ReferenceType);

    //! Called by Factory
    GameObject(unsigned gameObjectID);
    ~GameObject();

    Component* AddComponent(std::string name);
    Component* AddZilchComponent(std::string name);

    Component* GetComponent(std::string component);


    //Public Variables
    std::string Name;
    GameObject* Parent;
    const unsigned GameObjectID;


    //Components
    ComponentPointer(Transform);
    ComponentPointer(Sprite);
	  ComponentPointer(ShapeCollider);
    ComponentPointer(Camera);
    ComponentPointer(RigidBody);
    ComponentPointer(CharacterController);

  private:

    typedef std::map<std::string, Component *> ComponentMap;
    ComponentMap Components;

  };
}

