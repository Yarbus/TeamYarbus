/******************************************************************************/
/*!
\file   CharacterController.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Gamelogic for the player character
*/
/******************************************************************************/

#include <Precompiled.h>
#include "CharacterController.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "EventSystem.h"
#include "CollisionEvent.h"
#include "KeyEvent.h"
#include "UpdateEvent.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "InputManager.h"
#include "glfw3.h"
#include "Events.h"
#include "PhysicsLibrary.h"

META_DEFINE( Framework::CharacterController, CharacterController )
{

}

namespace Framework
{
  CharacterController* CharacterController::PLAYER = nullptr;

  ZilchDefineType (CharacterController, CinderZilch)
  {
    ZilchBindStaticFieldGetAs(PLAYER, "Player");
    ZilchBindMethodAs(ToggleFlying, "ToggleFlying");
    //type->HandleManager = ZilchManagerId (Zilch::PointerManager);
    //ZilchBindStaticMethodAs(ZGetCameraMousePosition, "GetCameraMousePosition");
  }

  static bool onGround = false;

  CharacterController::CharacterController ()
  {
    PLAYER = this;
    OBJECTSYSTEM->ptrPlayer = this->gameObject;
    useFlying = false;
  }

  CharacterController::~CharacterController ()
  {
    EVENTSYSTEM->mDisconnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mDisconnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);
    PLAYER = nullptr;
    OBJECTSYSTEM->ptrPlayer = nullptr;
  }

  void CharacterController::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* element = data->FindElement(data, "MicrophoneMultiplier");
    if (element)
      element->GetValue(&micMultiplier);
    else
      micMultiplier = vec2(0.0f, 10.0f);
    
    element = data->FindElement(data, "Acceleration");
    if (element)
      element->GetValue(&acceleration);
    else
      acceleration = vec2(50.0f, 20.f);
    
    element = data->FindElement(data, "JumpVelocity");
    if (element)
      element->GetValue(&jumpVel);
    else
      jumpVel = vec2(0.0f, 10.0f);
    
    element = data->FindElement(data, "UseFlying");
    if (element)
      element->GetValue(&useFlying);
    else
      useFlying = false;
  }

  void CharacterController::Initialize ()
  {
    PLAYER = this;
    OBJECTSYSTEM->ptrPlayer = this->gameObject;

    startingPos = gameObject->C<Transform>()->GetPosition2D();

    EVENTSYSTEM->mConnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mConnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);

    Transform* tform = AUDIOSYSTEM->listener = static_cast<Transform*>(gameObject->GetComponent("Transform"));
  }

  void CharacterController::ResetPosition()
  {
    gameObject->C<Transform>()->SetPosition(startingPos);
    RigidBody2D* rb = gameObject->C<RigidBody2D>();
    rb->velocity = vec2();
    rb->acceleration = vec2();
    rb->torque = rb->angularVelocity = 1.0f;
  }

  static void UpdateGroundState(CollisionEvent* collision)
  {
    glm::vec2 normal = glm::vec2(collision->normal);
    if (Physics::Angle_from_Vertical(normal) < 3.14f / 3.f)
      onGround = true;
    else
      onGround = false;
  }

  void CharacterController::OnCollisionEnter (CollisionEvent* collision)
  {
    UpdateGroundState(collision);
  }


  void CharacterController::Update(UpdateEvent* e)
  {
    RigidBody2D* body = static_cast<RigidBody2D*>(gameObject->AddComponent("RigidBody2D"));
    
    ErrorIf(!body, "ERROR, Character controller is missing RigidBody2D");

    Health* hp = static_cast<Health*>(gameObject->GetComponent("Health"));
    // if we have died
    if (hp && !hp->alive)
    {
      return;
    }

    // Jump/Fly up
    if (InputManager::IsKeyDown(GLFW_KEY_UP) && (onGround || useFlying) && (body->velocity.y < 9.5f))
    {
      onGround = false;
      body->ApplyForce(vec2(0, 1000 * body->mat->density));
        
      if (hp)
      {
        hp->currentDeathRate = hp->startingDeathRate * 20.0f; // only when you jump
      }
    }
    else // we are not flying
    {
      if (hp)
      {
        hp->currentDeathRate = hp->startingDeathRate;
      }
    }

    // Go down

    if (InputManager::IsKeyDown(GLFW_KEY_DOWN) && !onGround && (body->velocity.y > -9.5f))
    {
      body->ApplyForce(vec2(0, -2000 * body->mat->density));
    }

    if (InputManager::IsKeyDown(GLFW_KEY_RIGHT) && body->velocity.x < 50.0f)
      body->ApplyForce(vec2(acceleration.x * body->mat->density, 0.0f));
 
    if (InputManager::IsKeyDown(GLFW_KEY_LEFT) && body->velocity.x > -50.0f)
      body->ApplyForce(vec2(-acceleration.x * body->mat->density, 0.0f));
    
    // Microphone input
    gridPos = static_cast<Transform*>(gameObject->GetComponent("Transform"))->GetGridPosition();
    float micValue = AUDIOSYSTEM->GetMicrophoneValue ();
    body->ApplyForce(vec2(micValue * body->mat->density * micMultiplier));
    //Physics::THERMODYNAMICS->SetCellTemperature (gridPos.x, gridPos.y, 
    //  /*micValue **/ 10 * Constant::BT_Organics, e->Dt);
    //glm::vec2 vel = Physics::THERMODYNAMICS->GetCellVelocity(gridPos.x, gridPos.y);
    //body->ApplyForce(Vector2(10 * vel.x, 10 * vel.y));
  }

  bool CharacterController::CanFly() const
  {
    return useFlying;
  }

  void CharacterController::ToggleFlying()
  {
    useFlying = !useFlying;
  }

  DefineComponentName (CharacterController);
}
