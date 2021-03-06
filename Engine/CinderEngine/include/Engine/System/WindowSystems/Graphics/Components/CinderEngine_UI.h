/******************************************************************************/
/*!
\file   CinderEngine_UI.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_UI_H
#define _CINDER_ENGINE_UI_H

#include "Component.h"
#include "GraphicsCommon.h"
#include "IGraphicsObject.h"

namespace Framework
{
  class UpdateEvent;
  class UIEvent;

  struct Rect
  {
    glm::vec2 Dimension;
    glm::vec2 Min;
    glm::vec2 Max;
    glm::vec2 S_Min;
    glm::vec2 S_Max;

    bool Intersects (glm::vec2 v);
  };


  class UIComponent : public Component
  {
  public:
    UIComponent ();
    virtual ~UIComponent ();
    virtual void UIUpdate (UpdateEvent*) = 0;
    virtual void UIDraw () = 0;

  private:
    void Register ();
    void Deregister ();
  };


  class UIBox : public UIComponent
  {
  public:
    UIBox (){}
    virtual ~UIBox ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    virtual void UIUpdate (UpdateEvent*);
    virtual void UIDraw ();

    const static std::string Name;

  private:
    Rect box;
    glm::vec4 color;
    glm::vec4 normalColor;
    glm::vec4 hoverColor;
    glm::vec4 downColor;

    Shader* shader;
    Texture* texture;
    int buttonState;
    std::string Message;

    static VAO* vao;
    static VBO* vbo;
    static EBO* ebo;
    static UIEvent* uiEvent;

    void Specify_Attributes ();
  };  
}

#endif