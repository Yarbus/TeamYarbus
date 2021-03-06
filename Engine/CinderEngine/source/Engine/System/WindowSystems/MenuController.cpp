/******************************************************************************/
/*!
\file   MenuController.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Common.h"
#include "MenuController.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "UpdateEvent.h"
#include "PingEvent.h"
#include "Events.h"

namespace Framework
{


  /******************************************/
  // Menu Controller
  /******************************************/
  MenuController::~MenuController()
  {

  }

  void MenuController::Initialize()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, MenuController>(Events::UPDATEEVENT, this, &MenuController::MenuControllerUpdate);
    EVENTSYSTEM->mConnect<PingEvent, MenuController>(Events::PING_GAMEOBJECT, this, &MenuController::AddButtonsWhoPing);
  }

  void MenuController::Serialize(Serializer::DataNode* data)
  {

  }

  // Add Buttons who ping
  void MenuController::AddButtonsWhoPing(PingEvent * e)
  {
    // if the ping is a button
    if (!e || !e->Ping)
      return;

    ButtonController*bc = static_cast<ButtonController*>(e->Ping->GetComponent("ButtonController"));
    if (bc)
    {
      Button new_button;
      new_button.ActiveState =bc->ActiveState;
      new_button.Obj = e->Ping;
      Buttons.push_back(new_button);
    }
  }

  // Function to check mouse position and collision with buttons
  void MenuController::MenuControllerUpdate(UpdateEvent* e)
  {

  }



  /******************************************/
  // Button Controller
  /******************************************/









} // namespace Framework