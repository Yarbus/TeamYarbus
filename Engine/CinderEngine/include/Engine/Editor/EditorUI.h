/******************************************************************************/
/*!
\file   EditorUI.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _EDITOR_UI_H
#define _EDITOR_UI_H

#include "AntTweakBar.h"

namespace Editor
{

  template <typename T> struct TypeToTW
  {
    static const TwType value{ TW_TYPE_UNDEF };
  };

  template <>	struct TypeToTW<int> { static const TwType value{ TW_TYPE_INT32 }; };
  template <>	struct TypeToTW<bool> { static const TwType value{ TW_TYPE_BOOLCPP }; };
  template <>	struct TypeToTW<float> { static const TwType value{ TW_TYPE_FLOAT }; };
  template <>	struct TypeToTW<double> { static const TwType value{ TW_TYPE_DOUBLE }; };

  template <typename T> void AddTweak (TwBar* tw, const char *name, T *var, const char *def)
  {
    TwAddVarRW (tw, name, TypeToTW<T>::value, var, def);
  }

  inline void AddTweakColor3f (TwBar* tw, const char *name, float *col, const char *def)
  {
    TwAddVarRW (tw, name, TW_TYPE_COLOR3F, col, def);
  }

  inline void AddTweakColor4f (TwBar* tw, const char *name, float *col, const char *def)
  {
    TwAddVarRW (tw, name, TW_TYPE_COLOR4F, col, def);
  }

  inline void AddTweakDir3f (TwBar* tw, const char *name, float *dir, const char *def)
  {
    TwAddVarRW (tw, name, TW_TYPE_DIR3F, dir, def);
  }

  inline void AddTweakText(TwBar* tw, const char* name, std::string* s, const char* def)
  {
    TwAddVarRW(tw, name, TW_TYPE_STDSTRING, (void*)s, def);
  }

  template <typename T> void AddVar (TwBar* tw, const char *name, T *var, const char *def)
  {
    TwAddVarRO (tw, name, TypeToTW<T>::value, var, def);
  }

  inline void RemoveVar (TwBar* tw, const char *name)
  {
    TwRemoveVar (tw, name);
  }

  inline void RemoveAllVars (TwBar* tw)
  {
    TwRemoveAllVars (tw);
  }

  inline void AddSeparator (TwBar* tw)
  {
    TwAddSeparator (tw, "sep", "");
  }
}

#endif