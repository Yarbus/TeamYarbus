/******************************************************************************/
/*!
\file   Console.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Holds all the logic for changing the color of the console text.
Also has functions to create the console.
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Common.h"

namespace CinderConsole
{
  /*!
  _textcolor           = current color of the printed text
  _backgroundcolor     = current color of the background around the printed test.*/
  ConsoleColors _textcolor, _backgroundcolor;

  /*!
  _deftextcolo        = The original color of the text.
  _defbackgroundcolor = The original color of the background around the printed text*/
  ConsoleColors _deftextcolor, _defbackgroundcolor;

  //! handle to the console
  HANDLE _hstdconsole;


  /*!Create the console and also sets up all the code for its color
  and input systems

  @TODO Incorporate a way to talk to event system through the hot key ' ` '*/
  void Create_Cinder_Console(const char* name)
  {
    AllocConsole();
    FILE *file;
    freopen_s(&file, "CONOUT$", "wt", stdout);
    freopen_s(&file, "CONOUT$", "wt", stderr);
    freopen_s(&file, "CONIN$", "rt", stdin);

    SetConsoleTitle(name);

    /*! getting a handle to the console*/
    _hstdconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleColors(_textcolor, _backgroundcolor);

    /*! Saving the original color of the console*/
    _deftextcolor = _textcolor;
    _defbackgroundcolor = _backgroundcolor;
  }

  /*!Free console over creating it it*/
  void Free_Cinder_Console()
  {
    FreeConsole();
  }

  /*!Function to get the console colors*/
  void GetConsoleColors(ConsoleColors &textcolor, ConsoleColors &backgroundcolor)
  {
    //! Get the console screen information
    CONSOLE_SCREEN_BUFFER_INFO CS_Buffer_info;
    GetConsoleScreenBufferInfo(_hstdconsole, &CS_Buffer_info);

    //! get first 4 bits for text color
    textcolor = ConsoleColors(CS_Buffer_info.wAttributes & 15);
    //! get last 4 bits for background color
    backgroundcolor = ConsoleColors(CS_Buffer_info.wAttributes & 0xf0);

  }

  /*!Set the text and background color*/
  void SetColor(const ConsoleColors textcolor, const ConsoleColors backgroundcolor)
  {
    if (PROTECTCOLORS && (textcolor == backgroundcolor))
      return;

    //!Set Both text color and background color
    _textcolor = textcolor;
    _backgroundcolor = backgroundcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)_backgroundcolor << 4) | (unsigned int)_textcolor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(_hstdconsole, wAttributes);
  }

  /*!Set the color of the text in the console*/
  void SetTextColor(const ConsoleColors textcolor)
  {
    if (PROTECTCOLORS && (textcolor == _backgroundcolor))
      return;

    _textcolor = textcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)_backgroundcolor << 4) | (unsigned int)_textcolor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(_hstdconsole, wAttributes);
  }

  /*!Set the background color of the console*/
  void SetBackgroundColor(const ConsoleColors backgroundcolor)
  {
    if (PROTECTCOLORS && (_textcolor == backgroundcolor))
      return;

    _backgroundcolor = backgroundcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)_backgroundcolor << 4) | (unsigned int)_textcolor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(_hstdconsole, wAttributes);
  }

  std::ostream& CinderConsole::operator<<(std::ostream& os, ConsoleColors color)
  {
    SetTextColor(color);
    return os;
  }

  std::istream& CinderConsole::operator>>(std::istream& is, ConsoleColors color)
  {
    SetTextColor(color);
    return is;
  }

}