/******************************************************************************/
/*!
\file   RigidBody2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "GameObject.h"
#include "RigidBody2D.h"
#include "Collider2D.h"

namespace Framework
{
  DefineComponentName (RigidBody2D);

  RigidBody2D::RigidBody2D()
  {
    gameObject = nullptr;
    matName = "";
    mat = new Material();
    force = velocity = vec2();
    maxVelocity = vec2(10, 10);
    angularVelocity = 0;
    torque = 0;
    mass = 0;
    invMass = 0;
    I = 0;
    invI = 0;
    orient = MyRandom(-PI, PI);
    r = MyRandom(0.2f, 1.0f);
    g = MyRandom(0.2f, 1.0f);
    b = MyRandom(0.2f, 1.0f);
  }
	
	RigidBody2D::~RigidBody2D ()
	{
    if (mat)
    {
      delete mat;
      mat = nullptr;
    }
	}
	
	
	void RigidBody2D::SetOrient( float radians )
	{
	  orient = radians;
    if (gameObject)
    {
      ShapeCollider2D* shape = gameObject->C<ShapeCollider2D>();
      if (!shape)
        __debugbreak();

      shape->SetOrient(radians);

    }
  }
	
	void RigidBody2D::Serialize (Framework::Serializer::DataNode* data)
	{
    Serializer::DataNode* value = data->FindElement (data, "Static");
    value->GetValue (&isStatic);

    glm::vec3 tempVel;
    value = data->FindElement (data, "Velocity");
    if (value)
    {
      value->GetValue(&tempVel);
      velocity.x = tempVel.x;
      velocity.y = tempVel.y;
    }

    string newMatName;
    value = data->FindElement (data, "Material");
    value->GetValue (&newMatName);

    SetMaterial(newMatName);
	}
	
  void RigidBody2D::SetMaterial(string newMatName)
  { 
    if (newMatName != "")
    {
      matName = Serializer::SkipHash(newMatName.c_str());

      Serializer::ZeroSerializer materialtype(matName);
      mat->Serialize(materialtype.GetTrunk()->branch);
      SetDensity(mat->density);
    }
  }

	void RigidBody2D::Initialize ()
  {
    PHYSICS->Add(this);
  }

  void RigidBody2D::ApplyForce (const vec2& f)
  {
    //if (force.y < 0.1f)
    force += f;

    //std::cout << force.y << "\n";
  }

  void RigidBody2D::SetDensity(const float& newDensity)
  {
    mat->density = newDensity;
    ComputeMass();
  }

  float RigidBody2D::GetArea()
  {
    if (gameObject)
    {
      ShapeCollider2D* shape = gameObject->C<ShapeCollider2D>();
      
      assert(shape && "shape invalid pointer Rigidbody2D.cpp");
      switch (shape->GetType())
      {
        case ShapeCollider2D::eCircle:
          return static_cast<CircleCollider2D*>(shape)->GetArea();
        case ShapeCollider2D::ePoly:
          return static_cast<PolygonCollider2D*>(shape)->GetArea();
        case ShapeCollider2D::eCompound:
          return static_cast<CompoundCollider2D*>(shape)->GetArea();
        default:
          Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
          return tform->GetScale().x * tform->GetScale().y;
      }
    }
    else
    {
      __debugbreak();
    }

    return 1.0f;
  }

  void RigidBody2D::ComputeMass()
  {
    mass = mat->density * GetArea();
    invMass = (mass > 0.0f) ? 1.0f / mass : 0.0f;

    ShapeCollider2D* shape = gameObject->C<ShapeCollider2D>();
    
    assert(shape && "No shape bwahaha");
    switch (shape->GetType())
    {
    case ShapeCollider2D::ColliderType::eCircle: {
      CircleCollider2D *circle = static_cast<CircleCollider2D*>(shape);
      I = mass * circle->GetRadius() * circle->GetRadius();
    }
      break;

    case ShapeCollider2D::ColliderType::ePoly: {
      PolygonCollider2D *poly = static_cast<PolygonCollider2D*>(shape);
      //TODO Implement the I for this collider. Not necessary at the time as our only polygon collider is static(TERRAIN).
    }
        break;

    case ShapeCollider2D::ColliderType::eCompound: {
      //TODO Only do this if you get the polygon one working. Should be relatively simple as you probably just use the sum of impulses somehow.
    }
        break;
    }

    invI = (abs(I) > 0) ? 1.0f / I : 0.0f;
  }

}

