/**************************************************************************/
/*!
\file   ShapeData.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  

As a condition of your accessing this area, you agree to be bound by the
following terms and conditions :

The games software was created by students of DigiPen Institute of
Technology(DigiPen), and all copyright and other rights in such is owned by
DigiPen.While DigiPen allows you to access, download and use the software
for non - commercial, home use you hereby expressly agree that you will not
otherwise copy, distribute, modify, or(to the extent not otherwise
permitted by law) decompile, disassemble or reverse engineer the games
software.

THE GAMES SOFTWARE IS MADE AVAILABLE BY DIGIPEN AS - IS AND WITHOUT
WARRANTY OF ANY KIND BY DIGIPEN.DIGIPEN HEREBY EXPRESSLY DISCLAIMS ANY SUCH
WARRANTY, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. WITHOUT LIMITING THE
GENERALITY OF THE FOREGOING, DIGIPEN SHALL NOT BE LIABLE IN DAMAGES OR
OTHERWISE FOR ANY DAMAGE OR INJURY FROM DOWNLOADING, DECOMPRESSING, RUNNING
OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY, THE
GAMES SOFTWARE CONTAINED IN THIS AREA, NOR SHALL DIGIPEN BE LIABLE FOR ANY
INCIDENTAL, CONSEQUENTIAL, EXEMPLARY OR OTHER TYPES OF DAMAGES ARISING FROM
ACCESS TO OR USE OF THE GAMES SOFTWARE.

YOU HEREBY AGREE TO INDEMNIFY, DEFEND AND HOLD HARMLESS DIGIPEN AND ITS
DIRECTORS, OFFICERS, EMPLOYEES, AGENTS, CONSULTANTS AND CONTRACTORS AGAINST
ALL LIABILITY OF ANY KIND ARISING OUT OF YOUR DOWNLOADING, DECOMPRESSING,
RUNNING OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY,
THE GAMES SOFTWARE. DIGIPEN MAKES NO WARRANTIES OR REPRESENTATIONS THAT THE
GAMES SOFTWARE IS FREE OF MALICIOUS PROGRAMMING, INCLUDING, WITHOUT
LIMITATION, VIRUSES, TROJAN HORSE PROGRAMS, WORMS, MACROS AND THE LIKE.AS
THE PARTY ACCESSING THE GAMES SOFTWARE IT IS YOUR RESPONSIBILITY TO GUARD
AGAINST AND DEAL WITH THE EFFECTS OF ANY SUCH MALICIOUS PROGRAMMING.
*/
/**************************************************************************/

#ifndef _SHAPE_DATA_H
#define _SHAPE_DATA_H

#include "VertexData.h"

namespace Framework
{
  struct ShapeData
  {
    ShapeData () : vertices (0), numVertices (0), indices (0), numIndices (0) {}

    void Clean ()
    {
      delete [] vertices;
      delete [] indices;
      vertices = nullptr;
      indices = nullptr;
      numIndices = numVertices = 0;
    }

    GLuint vbo_size ()
    {
      return numVertices * sizeof (VertexData);
    }

    GLuint ebo_size ()
    {
      return numIndices * sizeof (GLuint);
    }

    VertexData* vertices;
    GLuint numVertices;
    GLuint* indices;
    GLuint numIndices;
  };
}

#endif