#include "Common.h"
#include "CharacterController.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "TimeQuery.h"
#include "glmOverloads.h"
#include "PhysicsLibrary.h"
#include "IncludeForAllCollision.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "EventSystem.h"
#include "CollisionEvent.h"
#include "KeyEvent.h"
#include "UpdateEvent.h"
#include "RigidBody2D.h"
#include "Collider2D.h"


namespace Framework
{
  CharacterController* PLAYER = nullptr;
  static bool onGround = false;

  CharacterController::~CharacterController ()
  {
  }

  void CharacterController::OnKeyPressed (KeyEvent* _key)
  {
    //vec3 force, accelH = { 1.0f, 0.0f, 0.0f }, accelV = { 0.0f, 1.0f, 0.0f };
    switch (_key->KeyValue)
    {
    case GLFW_KEY_UP:
      //gameObject->RigidBody->vel.y = jumpVel;
      //Physics::applyAccel(accelV, 0.016);
      //gameObject->Transform->Translate(0, 1, 0);
      
      if (onGround)
      {
        gameObject->RigidBody2D->velocity.y += jumpVel.y;
        onGround = false;
      }
      break;

    case GLFW_KEY_RIGHT:
      if (gameObject->RigidBody2D->force.x < 0.4f && gameObject->RigidBody2D->force.x > -0.4f)
      gameObject->RigidBody2D->ApplyForce
        (
        Vector2
        (
        acceleration.x * density,
        acceleration.y * density
        )
        );
      //if (gameObject->RigidBody->vel.x <= maxXVel)
      //  gameObject->RigidBody->vel.x += accel*0.016f;
      //gameObject->Transform->Translate(1, 0, 0);
      break;

    case GLFW_KEY_LEFT:
      if (gameObject->RigidBody2D->force.x < 0.4f && gameObject->RigidBody2D->force.x > -0.4f)
      gameObject->RigidBody2D->ApplyForce
        (
        Vector2
        (
        -acceleration.x * density,
        acceleration.y * density
        )
        );
      break;

    case GLFW_KEY_R:
    {glm::vec2 position = glm::vec2 (0, 500);
    gameObject->RigidBody2D->position = Vector2 (position.x, position.y);
    gameObject->RigidBody2D->angularVelocity = 0.0f;
    gameObject->RigidBody2D->velocity = Vector2 (0, 0);
    gameObject->RigidBody2D->force = Vector2 (0, 0);
    Camera::main->gameObject->Transform->SetPosition (position.x, position.y);
    //if (gameObject->RigidBody->vel.x >= -maxXVel)
    //  gameObject->RigidBody->vel.x -= accel*0.016f;
    //gameObject->Transform->Translate(-1, 0, 0);
    }
      break;

    default:
      break;
    }


    //if (_key->KeyDown)
    //{
    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_A))
    //  {
    //    vec3 force, accel;
    //    force = { -5.0f, 0 }; //WASD control    
    //    //float mass = 2;
    //    //force should be multiplied by friction to get smaller every update
    //    accel = Physics::getAccel (force, 0.01);


    //  }

    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_D))
    //    gameObject->Transform->Translate (0.01f, 0, 0);

    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_S))
    //
    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_W))
    //
    //  //if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
    //  //  useDebug = !useDebug;
    //}
    ////////////////////////////////////////////////////////////////////////
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

  static float t = 1;

  void CharacterController::Update (UpdateEvent* e)
  {
    gridPos = gameObject->Transform->GetGridPosition ();
    float micValue = AUDIOSYSTEM->GetMicrophoneValue ();
    //std::cout << micValue << "\n";
    gameObject->RigidBody2D->ApplyForce
      (
      Vector2
      (
      micValue * microhponeMultiplier.x * density,
      micValue * microhponeMultiplier.y * density
      )
      );
    Physics::THERMODYNAMICS->SetCellTemperature (gridPos.x, gridPos.y, 400000, 0.016);

    ////how to get line collider
    ////gameObject->CircleCollider->DetectLine(gameObject->LineCollider);
    ////go1->CircleCollider->DetectCircle(go2->CircleCollider);
    //vec3 dragDirection = -gameObject->RigidBody->vel;
    //if (gameObject->RigidBody->vel.x != 0 && gameObject->RigidBody->vel.y != 0)
    //  dragDirection = glm::normalize(dragDirection);
    //gameObject->RigidBody->vel += Physics::applyAccel(dragDirection * drag, .016);
    //gameObject->Transform->Translate(gameObject->RigidBody->vel.x * e->Dt, gameObject->RigidBody->vel.y * e->Dt, 0);
    //std::cout << AUDIOSYSTEM->input.peaklevel[0] << std::endl;
    //if (currentforce <= 1 && AUDIOSYSTEM->input.peaklevel[0] > 0.07)
    //  currentforce += AUDIOSYSTEM->input.peaklevel[0] * 2;
    //else if (currentforce > 0 && AUDIOSYSTEM->input.peaklevel[0] <= 0.07)
    //  currentforce -= AUDIOSYSTEM->input.peaklevel[0];

    //gameObject->RigidBody->vel.y = currentforce * jumpVel;
  }

  //void CharacterController::UpdateVelocity()
  //{
  //}


  /*!Telegraph that the component is active*/
  void CharacterController::Initialize ()
  {
    PLAYER = this;
    //accel = { 0 , 0 };
    //maxAcceleration = { 50, 100 };
    //maxXVel = 2.0f;
    //drag = 5;
    //currentforce = 0;
    density = gameObject->ShapeCollider2D->Density;

    EVENTSYSTEM->mConnect<KeyEvent, CharacterController> (Events::KEY_ANY, this, &CharacterController::OnKeyPressed);
    EVENTSYSTEM->mConnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mConnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);

    AUDIOSYSTEM->listener = gameObject->Transform;
  }

  void CharacterController::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "MicrophoneMultiplier");
    value->GetValue (&microhponeMultiplier);

    value = data->FindElement (data, "Acceleration");
    value->GetValue (&acceleration);

    value = data->FindElement (data, "JumpVelocity");
    value->GetValue (&jumpVel);
  }

  DefineComponentName (CharacterController);
}
