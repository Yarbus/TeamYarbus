/******************************************************************************/
/*!
\file   TDLib.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "type_vec.hpp"

namespace Framework
{
  namespace Physics
  {
    namespace Constant
    {
      //Add physics constants here
      static const long  c   = 299792458;  //m.s-1       | Speed of light in vaccum
      static const long  atm = 101325;     //Pa          | Standard Atmospheric pressure
      static const float R   = 8.31451f;   //J.mol-1.K-1 | Molar gas constant
      static const float M_a = 28.97f;     //Kg.Kmol-1   | Molar mass of air

      //Properties of specific materials--------------------
      //Specific heat
      static const float c_Air = 1046.f;
      static const float c_Water = 4186.f;
      static const float c_Soil = 1046.f;
      static const float c_Stone = 858.f;
      static const float c_Iron = 452.f;
      static const float c_Steel = 452.f;
      static const float c_Lead = 128.f;
      static const float c_Wood = 1674.f;

      //Thermal Conductivity
      static const float K_Air = 0.024f;
      static const float K_Wood = 0.048f;
      static const float K_drySoil = 1.5f;
      static const float K_Bronze = 110.f;
      static const float K_Carbon = 1.7f;
      static const float K_cotton = 0.029f;
      static const float K_Cement = 1.73f;
      static const float K_Stone = 1.7f;
      static const float K_Copper = 401.f;

      //Heat transfer coefficient
      static const float Hc_Air = 10;
    } //namespace Constant

    enum Material
    {
      AIR,
      WATER,
      SOIL,
      STONE,
      LEAD,
      IRON,
      STEEL,
      WOOD,
      COTTON
    };

    float Buoyancy(float density, float mass, float CellSize);
    float ConductiveHeatTransfer(float K, float T1, float T2, const double dt, float Size);
    float ConvectiveHeatTransfer(float Hc, float T1, float T2, const double dt);
    float dTemp(float dQ, float m, float c);

  } //namespace Physics
} //namespace Framework
