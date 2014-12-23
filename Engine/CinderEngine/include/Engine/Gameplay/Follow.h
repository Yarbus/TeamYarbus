/******************************************************************************/
/*!
\file   Follow.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FOLLOW_H
#define _FOLLOW_H

namespace Framework
{
  class UpdateEvent;
  class Transform;

  class Follow : public Component
  {
  public:
  Follow ();
  ~Follow ();

  virtual void Initialize ();
  virtual void Serialize (Serializer::DataNode* data);
  virtual void OnApplicationPause (PauseEvent* pause);
  void Update (UpdateEvent* update);

  const static std::string Name;

  private:
    std::string targetName;
    Transform* target;
  };  
}

#endif