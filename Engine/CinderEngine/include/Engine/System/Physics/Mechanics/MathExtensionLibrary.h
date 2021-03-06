/******************************************************************************/
/*!
\file   MathExtensionLibrary.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

// Reference http://gamedevtutsplusplus.com

#ifndef _MATH_EXT_H
#define _MATH_EXT_H


#include <cmath> // abs, sqrt
#include <cassert> // assert
#include <algorithm> // max, min

const float PI = 3.141592741f;
const float EPSILON = 0.0001f;
/*
struct Vector2
{
  union
  {
    float m[1][1];
    float v[2];

    struct
    {
      float x;
      float y;
    };
  };

  Vector2( ) {}
  Vector2(vec2 v) { x = v.x; y = v.y; }
  Vector2( float x_, float y_ )
    : x( x_ )
    , y( y_ )
  {
  }

  void Set( float x_, float y_ )
  {
    x = x_;
    y = y_;
  }

  Vector2 operator-( void ) const
  {
    return Vector2( -x, -y );
  }

  Vector2 operator*( float s ) const
  {
    return Vector2( x * s, y * s );
  }

  Vector2 operator/( float s ) const
  {
    return Vector2( x / s, y / s );
  }

  void operator*=( float s )
  {
    x *= s;
    y *= s;
  }

  Vector2 operator+( const Vector2& rhs ) const
  {
    return Vector2( x + rhs.x, y + rhs.y );
  }

  Vector2 operator+( float s ) const
  {
    return Vector2( x + s, y + s );
  }

  void operator+=( const Vector2& rhs )
  {
    x += rhs.x;
    y += rhs.y;
  }

  Vector2 operator-( const Vector2& rhs ) const
  {
    return Vector2( x - rhs.x, y - rhs.y );
  }

  void operator-=( const Vector2& rhs )
  {
    x -= rhs.x;
    y -= rhs.y;
  }

  float LenSqr( void ) const
  {
    return x * x + y * y;
  }

  float Len( void ) const
  {
    return std::sqrt( x * x + y * y );
  }

  void Rotate( float radians )
  {
    float c = std::cos( radians );
    float s = std::sin( radians );

    float xp = x * c - y * s;
    float yp = x * s + y * c;

    x = xp;
    y = yp;
  }

  void Normalize( void )
  {
    float len = Len( );

    if(len > EPSILON)
    {
      float invLen = 1.0f / len;
      x *= invLen;
      y *= invLen;
    }
  }
};

inline Vector2 operator*( float s, const Vector2& v )
{
  return Vector2( s * v.x, s * v.y );
}

*/
/*
struct Mat2
{
  union
  {
    struct
    {
      float m00, m01;
      float m10, m11;
    };

    float m[2][2];
    float v[4];
  };

  Mat2( ) {}
  Mat2( float radians )
  {
    float c = std::cos( radians );
    float s = std::sin( radians );

    m00 = c; m01 = -s;
    m10 = s; m11 =  c;
  }

  Mat2( float a, float b, float c, float d )
    : m00( a ), m01( b )
    , m10( c ), m11( d )
  {
  }

  void Set( float radians )
  {
    float c = std::cos( radians );
    float s = std::sin( radians );

    m00 = c; m01 = -s;
    m10 = s; m11 =  c;
  }

  Mat2 Abs( void ) const
  {
    return Mat2( std::abs( m00 ), std::abs( m01 ), std::abs( m10 ), std::abs( m11 ) );
  }

  Vector2 AxisX( void ) const
  {
    return Vector2( m00, m10 );
  }

  Vector2 AxisY( void ) const
  {
    return Vector2( m01, m11 );
  }

  Mat2 Transpose( void ) const
  {
    return Mat2( m00, m10, m01, m11 );
  }

  const Vector2 operator*( const Vector2& rhs ) const
  {
    return Vector2( m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y );
  }

  const Mat2 operator*( const Mat2& rhs ) const
  {
    // [00 01]  [00 01]
    // [10 11]  [10 11]

    return Mat2(
      m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
      m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
      m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
      m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
    );
  }
};

inline Vector2 Min( const Vector2& a, const Vector2& b )
{
  return Vector2( std::min( a.x, b.x ), std::min( a.y, b.y ) );
}

inline Vector2 Max( const Vector2& a, const Vector2& b )
{
  return Vector2( std::max( a.x, b.x ), std::max( a.y, b.y ) );
}

inline float Dot( const Vector2& a, const Vector2& b )
{
  return a.x * b.x + a.y * b.y;
}

inline float DistSqr( const Vector2& a, const Vector2& b )
{
  Vector2 c = a - b;
  return Dot( c, c );
}

inline Vector2 Cross( const Vector2& v, float a )
{
  return Vector2( a * v.y, -a * v.x );
}

inline Vector2 Cross( float a, const Vector2& v )
{
  return Vector2( -a * v.y, a * v.x );
}

inline float Cross( const Vector2& a, const Vector2& b )
{
  return a.x * b.y - a.y * b.x;
}
*/

inline vec2 Cross(const vec2& v, float a)
{
  return vec2(a * v.y, -a * v.x);
}

inline vec2 Cross(float a, const vec2&v)
{
  return vec2(-a * v.y, a * v.x);
}

inline float Cross(const vec2& a, const vec2& b)
{
  return a.x * b.y - a.y * b.x;
}

// Comparison with tolerance of EPSILON
inline bool Equal( float a, float b )
{
  // <= instead of < for NaN comparison safety
  return std::abs( a - b ) <= EPSILON;
}

inline float Sqr( float a )
{
  return a * a;
}

inline float Clamp( float min, float max, float a )
{
  if (a < min) return min;
  if (a > max) return max;
  return a;
}

inline int Round( float a )
{
  return (int)(a + 0.5f);
}

inline float MyRandom( float l, float h )
{
  float a = (float)rand( );
  a /= RAND_MAX;
  a = (h - l) * a + l;
  return a;
}

inline bool BiasGreaterThan( float a, float b )
{
  const float k_biasRelative = 0.95f;
  const float k_biasAbsolute = 0.01f;
  return a >= b * k_biasRelative + a * k_biasAbsolute;
}

const float gravityScale = 5.0f;
const vec2 gravity( 0.0f, -10.0f * gravityScale);
const float dt = 1.0f / 60.0f;

inline glm::mat2 GetRotationMatrix(float radians)
{
  glm::mat2 m;
  float c = std::cos(radians);
  float s = std::sin(radians);

  m[0][0] = c; m[0][1] = -s;
  m[1][0] = s; m[1][1] = c;
  return m;
}
#endif // _MATH_EXT_H
