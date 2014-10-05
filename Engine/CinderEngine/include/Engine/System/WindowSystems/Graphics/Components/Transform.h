/******************************************************************************/
/*!
\file   Transform.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"

namespace Framework
{
  enum MatrixModes
  {
    MODEL_MATRIX = 0,
    VIEW_MATRIX,
    PROJECTION_MATRIX
  };

  class Transform : public Component
  {
  public:
    Transform () {}
    Transform (GameObject* go);
    ~Transform ();

    virtual void Initalize ();
    virtual void Serialize ();

    //////////////////////////////////////////////////////////////////////////
    // Transformations
    void Load_Identity ();
    void Translate (float x, float y, float z);
    void Scale (float x, float y, float z);
    void Scale (float v);
    void Rotate (float angle);
    //////////////////////////////////////////////////////////////////////////
    bool MatrixMode (int m);
    void UpdateMatrices ();

    //getters
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
    glm::vec3 GetPosition ();
    glm::vec3 GetScale ();
    float GetRotation ();

    static void Print (glm::vec3 position);

    // The non-base component usees DefineComponentName macro to name component
    const static std::string Name;
  private:
    int currentMatrix;
    //Matrix Stack
    // Useful When Using Parent-Child Structure For Complex Objects
    // Eg. Robot, Objects with many parts as children
    std::vector <glm::mat4> modelMatrix;
    std::vector <glm::mat4> modelViewProjectionmatrix;
    void push_matrix ();
    void pop_matrix ();
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
    glm::mat3 normalMatrix;

    // To avoid Unnecesary calculation in Update Matrices
    bool matricesReady;
  };
}

#endif