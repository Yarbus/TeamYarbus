/******************************************************************************/
/*!
\file   Fluid_Physics_Integration.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Fluid_Physics_Integration_Predict.h"
#include "Math_Vector2D.h"
#include "Cinder_Engine_Data_Types.h"
#include "Fluid_Physics_Integration.h"
#include "Fluid_Physics_Object.h"

namespace Framework
{
  void Fluid_Physics_Predict::UpdateVelocity (
    Fluid_Physics_Object & game_entity,
    const REAL32 delta_time
    )
  {
    MATH_VECTOR_2D new_position;

    new_position
      = game_entity.GetPosition ()
      + game_entity.GetForce () * delta_time;
    game_entity.SetVelocity ((
      new_position
      - game_entity.GetOldPosition ()
      ) / delta_time
      );
  }


  void Fluid_Physics_Predict::UpdatePosition (
    Fluid_Physics_Object & game_entity,
    const REAL32 delta_time
    )
  {
    game_entity.SetPosition (game_entity.GetPosition ()
      + game_entity.GetForce ()
      * delta_time);
    game_entity.SetOldPosition (game_entity.GetPosition ());
    game_entity.SetPosition (
      game_entity.GetPosition ()
      + game_entity.GetVelocity () * delta_time);
  }
}