/*****************************************************************************/
/*!
\file   Terrain3D.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

#include "Terrain3D.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Thermodynamics.h"
#include "random.hpp"

namespace Framework
{
  static std::vector<float> surfaceVertices;
  static VAO* vao1;
  static VBO* vbo1;

  DefineComponentName(Terrain3D);

  //Constructor
  Terrain3D::Terrain3D()
  {}

  //Destructor
  Terrain3D::~Terrain3D()
  {
    delete vao, vbo, tc;
    delete vao1, vbo1;
  }

  void Terrain3D::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement(data, "MapSize");
    value->GetValue(&MapSize);

    value = data->FindElement(data, "MapDepth");
    value->GetValue(&MapDepth);

    value = data->FindElement(data, "BaseHeight");
    value->GetValue(&BaseHeight);

    value = data->FindElement(data, "PeakHeight");
    value->GetValue(&PeakHeight);

    value = data->FindElement(data, "Passes");
    value->GetValue(&Passes);

    value = data->FindElement(data, "Waves");
    value->GetValue(&Waves);

    value = data->FindElement(data, "Color");
    value->GetValue(&color);
  }

  void Terrain3D::Initialize()
  {
    gameObject->Terrain3D = this;
    Generate_Height_Points();
    Generate_Vertices();
    Generate_Buffers();

    vao1 = new VAO();
    for (unsigned i = 0; i < height_points.size() - 1; ++i)
    {
      surfaceVertices.push_back(height_points[i].x);
      surfaceVertices.push_back(height_points[i].y);
      surfaceVertices.push_back(height_points[i].z);
      surfaceVertices.push_back(height_points[i + 1].x);
      surfaceVertices.push_back(height_points[i + 1].y);
      surfaceVertices.push_back(height_points[i + 1].z);
    }

    vbo1 = new VBO(surfaceVertices.size() * sizeof(float), surfaceVertices.data());
    GLint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    vao1->unbindVAO();
  }

  void Terrain3D::Draw()
  {
    shader->Use();
    vao->bindVAO();

    shader->uni4f("color", color.r, color.g, color.b, color.a);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

    vao->unbindVAO();

    vao1->bindVAO();

    shader->uni4f("color", 1, 1, 1, 1.0f);

    vao1->unbindVAO();
    shader->Disable();
  }

  void Terrain3D::Generate_Height_Points()
  {
    std::vector<float> heights;
    tc = new Procedural::TerrainCreator3D(MapSize, MapDepth, BaseHeight, Passes, Waves, PeakHeight);
    Procedural::TerrainCreator3D& t = *tc;
    float** Map = t.GetMap();
   
    float offsetX = -1.0f;
    float offsetY = -1.0f;
    float offsetZ = -1.0f;

    float nX = 2.0f / (MapSize - 1);
    float nY = 2.0f / (MapSize - 1);
    float nZ = 2.0f / (MapDepth - 1);
    float previousHeight = -1.0f;
    float MaxHeight = BaseHeight + PeakHeight;

    for (int i = 0; i < MapSize; ++i)
    {
      for (int j = 0; j < MapDepth; ++j)
      {
        if (previousHeight != offsetY || i == MapSize - 1 || j == MapDepth - 1)
        {
          height_points.push_back({ offsetX, offsetY / MaxHeight, offsetZ });
          previousHeight = offsetY;
        }

        offsetY = (Map[j][i]) / 2.0f;
        if (offsetY < 0)
          offsetY = 0.0f;

        offsetZ += 4 * nZ;
      }
      offsetX += 4 * nX;
    } //for
  } //function generate height points

  /*bool CalculateIntersectionPoint
    (
    double Ax, double Ay, double Az,
    double Bx, double By, double Bz,
    double Cx, double Cy, double Cz,
    double Dx, double Dy, double Dz,
    double *X, double *Y, double *Z
    )
  {

  }*/

  void Terrain3D::Generate_Vertices()
  {
    //Vertices
    for (unsigned i = 0; i < height_points.size() - (MapDepth + 1); ++i)
    {
      //Triangle 1
      //pt1
      vertices.push_back(height_points[i].x);
      vertices.push_back(height_points[i].y);
      vertices.push_back(height_points[i].z);
      //pt2
      vertices.push_back(height_points[i + 1].x);
      vertices.push_back(height_points[i + 1].y);
      vertices.push_back(height_points[i + 1].z);
      //pt3
      vertices.push_back(height_points[i + MapDepth].x);
      vertices.push_back(height_points[i + MapDepth].y);
      vertices.push_back(height_points[i + MapDepth].z);
      //Triangle 2
      //pt1
      vertices.push_back(height_points[i + 1].x);
      vertices.push_back(height_points[i + 1].y);
      vertices.push_back(height_points[i + 1].z);
      //pt2
      vertices.push_back(height_points[i + MapDepth].x);
      vertices.push_back(height_points[i + MapDepth].y);
      vertices.push_back(height_points[i + MapDepth].z);
      //pt3
      vertices.push_back(height_points[i + MapDepth + 1].x);
      vertices.push_back(height_points[i + MapDepth + 1].y);
      vertices.push_back(height_points[i + MapDepth + 1].z);
    }
  }

  void Terrain3D::Generate_Buffers()
  {
    vao = new VAO();
    shader = Resources::RS->Get_Shader("Terrain");
    shader->Use();

    vbo = new VBO(vertices.size() * sizeof(float), vertices.data());
    GLuint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    vao->unbindVAO();
  }

} //framework