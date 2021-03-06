/******************************************************************************/
/*!
\file   EndTrigger.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  DefineComponentName (EndTrigger);

  // Constructor
  EndTrigger::EndTrigger ()
  {}
  
  // Destructor
  EndTrigger::~EndTrigger ()
  {}

  

  void EndTrigger::Initialize ()
  {
    EVENTSYSTEM->mConnect<CollisionEvent, EndTrigger> (Events::COLLISION, this, &EndTrigger::OnCollisionEnter);
    EVENTSYSTEM->mConnect <UpdateEvent, EndTrigger>(Events::UPDATEEVENT, this, &EndTrigger::Update);
  }

  void EndTrigger::Update(UpdateEvent* update)
  {
    Transform* characterTransform = static_cast<Transform*>(CharacterController::PLAYER->gameObject->GetComponent("Transform"));
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    if (!triggered && characterTransform->GetPosition().x >= tform->GetPosition().x)
    {
      triggered = true;
      BaseEvent b;
      EVENTSYSTEM->TriggerEvent(Events::END_EVENT, b);
    }
  }

  void EndTrigger::Serialize (Serializer::DataNode* data)
  {
  }

  void EndTrigger::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void EndTrigger::OnCollisionEnter (CollisionEvent* coll)
  {
    //if (!triggered && coll->thisObject != nullptr && coll->OtherObject->Name == "Player")
    //{
    //  if (coll->thisObject->Name == gameObject->Name)
    //  {
    //    triggered = true;
    //    std::cout << "TRIGGERED\n";
    //    BaseEvent b;
    //    EVENTSYSTEM->TriggerEvent (Events::END_EVENT, b);
    //  }
    //}
  }

}