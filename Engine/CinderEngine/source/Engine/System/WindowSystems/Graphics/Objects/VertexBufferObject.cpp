/******************************************************************************/
/*!
\file   VertexBufferObject.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "VertexBufferObject.h"

namespace Framework
{
  void VertexBufferObject::createVBO (int _size)
  {
    glGenBuffers (1, &vbo);
    data.reserve (_size);
    bufferSize = _size;
  }

  /*-----------------------------------------------

  Name:		releaseVBO

  Params:	none

  Result:	Releases VBO and frees all memory.

  /*---------------------------------------------*/

  void VertexBufferObject::releaseVBO ()
  {
    glDeleteBuffers (1, &vbo);
    bDataUploaded = false;
    data.clear ();
  }

  /*-----------------------------------------------

  Name:	mapBufferToMemory

  Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...

  Result:	Maps whole buffer data to memory and
  returns pointer to data.

  /*---------------------------------------------*/

  void* VertexBufferObject::mapBufferToMemory (int iUsageHint)
  {
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBuffer (bufferType, iUsageHint);
    return ptrRes;
  }

  /*-----------------------------------------------

  Name:	mapSubBufferToMemory

  Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
  uiOffset - data offset (from where should
  data be mapped).
  uiLength - length of data

  Result:	Maps specified part of buffer to memory.

  /*---------------------------------------------*/

  void* VertexBufferObject::mapSubBufferToMemory (int iUsageHint, GLuint uiOffset, GLuint uiLength)
  {
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBufferRange (bufferType, uiOffset, uiLength, iUsageHint);
    return ptrRes;
  }

  /*-----------------------------------------------

  Name:	unmapBuffer

  Params:	none

  Result:	Unmaps previously mapped buffer.

  /*---------------------------------------------*/

  void VertexBufferObject::unmapBuffer ()
  {
    glUnmapBuffer (bufferType);
  }

  /*-----------------------------------------------

  Name:	bindVBO

  Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)

  Result:	Binds this VBO.

  /*---------------------------------------------*/

  void VertexBufferObject::bindVBO (int _bufferType)
  {
    bufferType = _bufferType;
    glBindBuffer (bufferType, vbo);
  }

  /*-----------------------------------------------

  Name:	uploadDataToGPU

  Params:	iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...

  Result:	Sends data to GPU.

  /*---------------------------------------------*/

  void VertexBufferObject::uploadDataToGPU (int iDrawingHint)
  {
    glBufferData (bufferType, data.size (), &data [0], iDrawingHint);
    bDataUploaded = true;
    data.clear ();
  }

  /*-----------------------------------------------

  Name:		addData

  Params:	ptrData - pointer to arbitrary data
  uiDataSize - data size in bytes

  Result:	Adds arbitrary data to VBO.

  /*---------------------------------------------*/

  void VertexBufferObject::addData (void* ptrData, GLuint uiDataSize)
  {
    data.insert (data.end (), (unsigned char*) ptrData, (unsigned char*) ptrData + uiDataSize);
  }

  /*-----------------------------------------------

  Name:	getDataPointer

  Params:	none

  Result:	Returns data pointer (only before uplading).

  /*---------------------------------------------*/

  void* VertexBufferObject::getDataPointer ()
  {
    if (bDataUploaded)return NULL;
    return (void*) data [0];
  }

  /*-----------------------------------------------

  Name:	getBuffer

  Params:	none

  Result:	Returns VBO ID.

  /*---------------------------------------------*/

  GLuint VertexBufferObject::getBuffer ()
  {
    return vbo;
  }
}
