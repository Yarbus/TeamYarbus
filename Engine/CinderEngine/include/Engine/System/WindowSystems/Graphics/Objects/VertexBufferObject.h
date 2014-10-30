#ifndef _VBO_H
#define _VBO_H

#include "glew.h"
#include <vector>

namespace Framework
{

  class VertexBufferObject
  {
  public:
    template <typename T>
    VertexBufferObject (GLuint bufferSize, T* bufferData)
    {
      glGenBuffers (1, &vbo);
      glBindBuffer (GL_ARRAY_BUFFER, vbo);
      glBufferData (GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
    }

    VertexBufferObject () {}

    ~VertexBufferObject ()
    {
      glDeleteBuffers (1, &vbo);
    }

    void createVBO (int size_ = 0);
    void releaseVBO ();

    void* mapBufferToMemory (int usage);
    void* mapSubBufferToMemory (int usage, GLuint offset, GLuint stride);
    void unmapBuffer ();

    void bindVBO (GLenum type = GL_ARRAY_BUFFER);
    void unBindVBO ();
    void uploadDataToGPU (GLenum drawMode);

    void addData (void* data, GLuint size);

    void* getDataPointer ();
    GLuint getBuffer ();
    void createMultiple (int _size, GLuint* ptr);
  private:
    GLuint vbo;
    GLuint bufferSize;
    GLuint bufferType;
    std::vector<unsigned char> data;

    bool bDataUploaded;
  };

  typedef VertexBufferObject VBO;

}

#endif

