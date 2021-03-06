/******************************************************************************/
/*!
\file   WindowSystem.h
\author Micah Rust, Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/

#pragma once

#include "BaseSystem.h"
#include "GraphicsCommon.h"
#include "glfw3.h"
#include "KeyEvent.h"


namespace Framework
{
  class IGraphicsObject;
  class UIComponent;

  namespace WindowNameSpace
  {
    void SetupKeyEvent(KeyEvent* triggered_key_event, const int& key, const int& scanCode, const int& state, const int& mod);
    void TriggerKeyEvent(const string eventname, const int& key, const int& scanCode, const int& state, const int& mod);
  }

  class WindowSystem : public BaseSystem
  {
  public:
	ZilchDeclareBaseType(WindowSystem, Zilch::TypeCopyMode::ReferenceType);
	  unsigned IsMouseDown;
    glm::dvec2 cursorPosition;
    WindowSystem(const char* WindowTitle, const int& ClientWidth = 1024, const int& ClientHeight = 768, const bool& fullscreen = true);
    ~WindowSystem();

    virtual bool Initialize ();
    virtual void Update(const float& dt);
    virtual const string GetName() { return "WindowSystem"; }

    void WindowsUpdate(const float& dt);
    void GraphicsUpdate(const float& dt);

    bool IsInFocus() const;
    void Restore();
    void Minimize();

    void ToggleCursorVisibility();
	  void SetCursorVisibility(bool set);
	  void KeyFunction(KeyEvent* key);
    // GLFW callbacks
    void GLFWMessageHandler(GLFWwindow* window, const int key, const int scanCode, const int state, const int mod);
	static void GLFWCharHandler( GLFWwindow* window, unsigned int glfwChar );
    void GLFWMouseButtonFunction(GLFWwindow *, const int button, const int action, const int mod);
    void GLFWMouseCursorMoved (GLFWwindow* window, const double xPos, const double yPos);
    void GLFWWindowClosed(GLFWwindow* window);
    void GLFWResize (GLFWwindow* window, const int w, const int h);
    void GLFWFrameBufferResize(GLFWwindow* _window, const int w, const int h);
    void GLFWWindowFocus(GLFWwindow* window, const int focus);

    int Get_Width ();
    int Get_Height ();
	
    glm::vec2 Get_Mouse_Position ();
	Zilch::Real2 ZGet_Mouse_Position();
    glm::vec2 Get_Normalized_Mouse_Position ();
	Zilch::Real2 ZGet_Normalized_Mouse_Position();
	//Zilch::Real2 ZGetMouseScreenPosition();
    inline GLFWwindow* Get_Window ()
    {
      return window;
    }
    void Set_W_H (const int& w, const int& h);

    bool focused = true;

	double dpi;

  private:
    GLFWwindow* window;

    glm::dvec2 lastCursorPos;

    int WindowHeight;
    int WindowWidth;
    bool cursorVisible;
	
  };

  extern WindowSystem* WINDOWSYSTEM;
}