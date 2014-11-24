/******************************************************************************/
/*!
\file   Camera.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Camera.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "BaseEvent.h"
#include "WindowSystem.h"
#include "GameObject.h"
#include "Pipeline.h"


namespace Framework
{
  DefineComponentName(Camera);

  std::list<Camera*> Camera::allCameras;
  Camera* Camera::current = NULL;
  Camera* Camera::main = NULL;

  Camera::~Camera ()
  {
    allCameras.remove (this);
    gameObject->Camera = nullptr;
    OPENGL->camera = nullptr;
  }


  void Camera::OnKeyPressed (KeyEvent* key)
  {
    float camSpeed = 0.25f;
    float zoomSpeed = 0.1f;
    if (key->KeyDown)
      switch (key->KeyValue)
    {
      case GLFW_KEY_A:
        Camera::main->gameObject->Transform->Translate (-camSpeed, 0, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_D:
        Camera::main->gameObject->Transform->Translate (camSpeed, 0, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_S:
        Camera::main->gameObject->Transform->Translate (0, -camSpeed, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_W:
        Camera::main->gameObject->Transform->Translate (0, camSpeed, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_Z:
        Camera::main->Zoom (zoomSpeed);
        break;
      case GLFW_KEY_X:
        Camera::main->Zoom (-zoomSpeed);
        break;
      default:
        break;
    }
  }


  void Camera::Initialize ()
  {
    EVENTSYSTEM->mConnect <KeyEvent, Camera> (Events::KEY_ANY, this, &Camera::OnKeyPressed);
    gameObject->Camera = this;
    allCameras.push_back(this);
    if (mainCamera)
    {
      Camera::main = this;
      worldToView = glm::lookAt(size * viewDirection + vec3 (gameObject->Transform->GetPosition().x, gameObject->Transform->GetPosition().y, 0.0), gameObject->Transform->GetPosition(), up);
      viewToProjection = glm::perspective(fov * M_PI / 180, aspect, nearPlane, farPlane);
    }
    Camera::current = this;

    OPENGL->camera = this;
    OPENGL->Perspective (fov, aspect, nearPlane, farPlane);
    OPENGL->MatrixMode (MODEL);
    OPENGL->Translatefv (const_cast <float*>(glm::value_ptr (gameObject->Transform->GetPosition ())));
    OPENGL->MatrixMode (VIEW);
    OPENGL->LookAt
      (size * viewDirection +
      glm::vec3 (gameObject->Transform->GetPosition ().x, gameObject->Transform->GetPosition ().y, 0.0),
      gameObject->Transform->GetPosition (), up);
    OPENGL->MatrixMode (MODEL);
    OPENGL->LoadIdentity ();
    matricesReady = true;
  }

  void Camera::Serialize(Serializer::DataNode* data)
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // viewDirection  : vec3 (Serialized Data)
    // aspect         : float     (Serialized Data)
    // nearPlane      : float     (Serialized Data)
    // farPlane       : float     (Serialized Data)
    // fov            : float     (Serialized Data)
    // main           : bool      (Serialized Data)
    //////////////////////////////////////////////////////////////////////////

    // Main?
    mainCamera = data->value_.Bool_;

	Serializer::DataNode* value;// = data->FindElement(data, "Facing");
	//value->GetValue(&viewDirection);

	viewDirection = { 0, 0, 1 };

    value = data->FindElement(data, "FieldOfView");
    value->GetValue(&fov);

    value = data->FindElement(data, "NearPlane");
    value->GetValue(&nearPlane);

    value = data->FindElement(data, "FarPlane");
    value->GetValue(&farPlane);

    value = data->FindElement (data, "Size");
    value->GetValue (&size);
	  aspect = (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height();
  }


  void Camera::MouseUpdate(const vec2& newPosition)
  {
    vec2 mouseDelta = newPosition - oldPosition;

    const float ROTATION_SPEED = 0.01f;
    const float MAX_DELTA = 150.0f;

    if (mouseDelta.length() < MAX_DELTA)
    {
      vec3 vRotation = glm::cross(viewDirection, up);
      glm::mat4 rotator = glm::mat4
        (glm::rotate(mouseDelta.x * ROTATION_SPEED, up) * glm::rotate(mouseDelta.y * ROTATION_SPEED, vRotation));
      viewDirection = glm::mat3(rotator) * viewDirection;
    }
    oldPosition = newPosition;
    matricesReady = false;
  }

  void Camera::UpdateCamera (Pipeline* p)
  {
    if (!matricesReady)
    {
      OPENGL->Perspective (fov, aspect, nearPlane, farPlane);
      OPENGL->LookAt
        (size * viewDirection +
        glm::vec3 (gameObject->Transform->GetPosition ().x, gameObject->Transform->GetPosition ().y, 0.0),
        gameObject->Transform->GetPosition(), up);
      OPENGL->MatrixMode (MODEL);
      OPENGL->LoadIdentity ();
    }
  }

  void Camera::UpdatePosition(const vec3& deltaPos)
  {
    position += deltaPos;
    matricesReady = false;
  }

  void Camera::Zoom(float zoom)
  {
    size += zoom;
    matricesReady = false;
  }

  glm::mat4 Camera::GetWorldToViewMatrix ()
  {
    //if (!Camera::main->matricesReady)
    {
      Camera::main->worldToView = glm::lookAt
        (
        Camera::main->size * Camera::main->viewDirection + vec3 (Camera::main->gameObject->Transform->GetPosition ().x, Camera::main->gameObject->Transform->GetPosition ().y, 0.0),
        Camera::main->gameObject->Transform->GetPosition (),
        Camera::main->up
        );
    }

    return Camera::main->worldToView;
  }

  glm::mat4 Camera::GetViewToProjectionMatrix ()
  {
    //if (!Camera::main->matricesReady)
    {
      Camera::main->viewToProjection =
      glm::perspective
      (
      Camera::main->fov * M_PI / 180,
      (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(),
      Camera::main->nearPlane,
      Camera::main->farPlane
      );
    }

    return Camera::main->viewToProjection;
  }

  float Camera::GetSize ()
  {
    return size;
  }

  float Camera::GetFOV ()
  {
    return fov;
  }

  glm::vec2 Camera::GetWorldMousePosition()
  {
    glm::vec2 ndc = { (WINDOWSYSTEM->Get_Mouse_Position ().x / WINDOWSYSTEM->Get_Width () - 0.5f) * 2.0f,
      ((WINDOWSYSTEM->Get_Height () - WINDOWSYSTEM->Get_Mouse_Position ().y) / WINDOWSYSTEM->Get_Height () - 0.5f) * 2.0f };
    /*
        destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
        destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;
    */
    return glm::vec2
      (
        ndc.x * Camera::main->size, ndc.y * Camera::main->size / Camera::main->aspect
      );
  }

  Zilch::Real2 Camera::GetCameraMousePosition ()
  {
    glm::vec2 ndc = { (WINDOWSYSTEM->Get_Mouse_Position ().x / WINDOWSYSTEM->Get_Width () - 0.5f) * 2.0f,
      ((WINDOWSYSTEM->Get_Height () - WINDOWSYSTEM->Get_Mouse_Position ().y) / WINDOWSYSTEM->Get_Height () - 0.5f) * 2.0f };
    /*
    destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;
    */
    return Zilch::Real2
      (
      ndc.x * Camera::main->size, ndc.y * Camera::main->size / Camera::main->aspect
      );
  }

}