/******************************************************************************/
/*!
\file   Effect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Effect.h"
#include <string>
#include "tunnelEffect.h"
#include "fountainEffect.h"
#include "attractorEffect.h"

namespace Framework
{
  std::shared_ptr<IEffect> EffectFactory::create (const char *name)
  {
    std::string effect{ name };

    if (effect == "tunnel")
      return std::make_shared<TunnelEffect> ();
    else if (effect == "attractors")
      return std::make_shared<AttractorEffect> ();
    else if (effect == "fountain")
      return std::make_shared<FountainEffect> ();

    return nullptr;
  }
}