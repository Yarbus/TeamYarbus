/******************************************************************************/
/*!
\file   GOC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once

#include "GameObject.h"
#include "ScriptComponent.h"
#include "Component.h"
#include "Zilch.hpp"

namespace Framework
{
  GameObject* Component::GetOwner() { return gameObject; }

  ZilchDefineType(Component, CinderZilch)
  {
    ZilchBindConstructor(Component);
    ZilchBindMethod(GetOwner);	
  }

  ZilchDefineType(GameObject, CinderZilch)
  {
    //Pointerrs
    ZilchBindFieldGet(Transform);
    ZilchBindFieldGet(Sprite);
    //ZilchBindFieldGetSet(ShapeCollider);
    //ZilchBindFieldGetSet(Camera);
    //ZilchBindFieldGetSet(RigidBody);
    //ZilchBindFieldGetSet(CharacterController);

    // 
    ZilchBindMethodOverload(AddComponent, Component*, Zilch::String);
    ZilchBindMethodOverload(AddZilchComponent, Component*, Zilch::String);
  }

  GameObject::GameObject(unsigned gameObjectID)
  :GameObjectID(gameObjectID)
  {
    Parent = NULL;
  }

  GameObject::~GameObject()
  {
    for each (auto comp in Components)
    {
      delete comp.second;
    }
  }

  // Zilch method for adding components to GameObjects
  Component* GameObject::AddComponent(Zilch::String name)
  {
    string stdname = name.c_str();
      ErrorIf(Components.find(stdname) != Components.end(), "COMPONENT CREATED TWICE ON SAME OBJECT");
    if (OBJECTSYSTEM->SerialMap.find(stdname) != OBJECTSYSTEM->SerialMap.end())
    {
      Component* gc = OBJECTSYSTEM->SerialMap[stdname]->Create();
      Components[stdname] = gc;
      gc->gameObject = this;
      gc->Initialize();
      return gc;
    }
    else
    {
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << std::endl;
      return NULL;
    }
  }

  Component* GameObject::AddComponent(string name)
  {
    ErrorIf(Components.find(name) != Components.end(), "COMPONENT CREATED TWICE ON SAME OBJECT");
    if (OBJECTSYSTEM->SerialMap.find(name) != OBJECTSYSTEM->SerialMap.end())
    {
      Component* gc = OBJECTSYSTEM->SerialMap[name]->Create();
      Components[name] = gc;
      gc->gameObject = this;
      return gc;
    }
    else
    {
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << std::endl;
      return NULL;
    }
  }

  Component* GameObject::AddZilchComponent(Zilch::String name)
  {
    string stdname = name.c_str();

    //not sure about error handling
    Component* zc = new ZilchComponent(stdname);
    return zc;
  }
  Component* GameObject::AddZilchComponent(string name)
  {
    //not sure about error handling
    Component* zc = new ZilchComponent(name);
    zc->Initialize();
    return zc;
  }

  Component* GameObject::GetComponent(string component)
  {
    ComponentMap::iterator it = Components.find(component);
    if (it == Components.end())
    {
      return NULL;
    }
    else
    {
      return it->second;
    }
    return NULL;
  }

}
