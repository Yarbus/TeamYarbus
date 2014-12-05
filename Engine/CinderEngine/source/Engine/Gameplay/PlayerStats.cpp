/******************************************************************************/
/*!
\file   PlayerStats.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "PlayerStats.h"
#include "EventSystem.h"
#include "ObjectSystem.h"
#include "GameEvent.h"
#include "UpdateEvent.h"
#include "Pipeline.h"

namespace Framework
{


  // Constructor
  PlayerStats::PlayerStats ()
  {}
  
  // Destructor
  PlayerStats::~PlayerStats ()
  {}

  void PlayerStats::Initialize ()
  {
    EVENTSYSTEM->mConnect<AllTreesBurnedEvent, PlayerStats> (Events::ALLLTREESBURNED, this, &PlayerStats::AllTreesBurned);
    EVENTSYSTEM->mConnect<UpdateEvent, PlayerStats> (Events::UPDATEEVENT, this, &PlayerStats::Update);
  }

  void PlayerStats::Serialize (Serializer::DataNode* data)
  {
    data->FindElement (data, "NextLevel")->GetValue (&NextLevel);
  }

  void PlayerStats::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void PlayerStats::AllTreesBurned (AllTreesBurnedEvent* atb)
  {
    OPENGL->Change_Shader ("FadeIn", (int) SS_FADE_OUT);
    levelComplete = true;
  }

  void PlayerStats::Update (UpdateEvent* update)
  {
    if (levelComplete)
    {
      timer += 0.016f;
      if (timer > 1.0f)
      {
        OBJECTSYSTEM->LoadLevel (NextLevel.c_str ());
      }
    }
  }

}