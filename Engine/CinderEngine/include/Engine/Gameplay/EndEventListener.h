/******************************************************************************/
/*!
\file   EndEventListener.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _END_EVENT_LISTENER_H
#define _END_EVENT_LISTENER_H

#include "MetaCreator.h"

namespace Framework
{
  class UpdateEvent;
  class EndEvent;
  class UIBox;
  class Sound;

  enum END_EVENT_STATE
  {
    EES_NONE,
    EES_CAMERA,
    EES_CREDITS,
  };

  class EndEventListener : public Component
  {
  public:

	  // META_DECLARE( EndEventListener );

  EndEventListener ();
  virtual ~EndEventListener ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void OnApplicationPause (PauseEvent* pause);
  void Update (UpdateEvent* update);
  void EndEventHandler (EndEvent* endEvent);

  const static string Name;

  private:
    Sound* creditsMusic;
    bool loadCredits = false;
    float timer = 0.0f;
    vec3 startPosition;
    vec3 endPosition;
    float startSize;
    float endSize;
    GameObject* exitGame = nullptr;
    END_EVENT_STATE ees = EES_NONE;
  };  
}

// META_DEFINE( Framework::EndEventListener, EndEventListener )
// {
// 
// }

#endif