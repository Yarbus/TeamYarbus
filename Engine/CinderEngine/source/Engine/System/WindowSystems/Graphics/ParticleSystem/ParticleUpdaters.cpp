/******************************************************************************/
/*!
\file   ParticleUpdaters.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  void EulerUpdater::init(int maxCount)
  {
    particleCount = maxCount;
    cs = Resources::RS->Get_ComputeShader ("ParticleSystem.cs.glsl");
    SSBOPos = new SSBO (maxCount * sizeof (vec4));

    vec4* vPos = SSBOPos->MapBufferRange<vec4> (0, maxCount);
    for (int i = 0; i < maxCount; ++i)
    {
      vPos [i] = { 0.5f, 0, 0, 0 };
    }
    SSBOPos->UnMapBuffer ();
    SSBOPos->BindBufferBase (0);
  }

  void EulerUpdater::update (double dt, ParticleData *p)
  {
    const vec4 globalA{ dt * m_globalAcceleration.x, dt * m_globalAcceleration.y, dt * m_globalAcceleration.z, 0.0 };
    const float localDT = (float) dt;

    vec4 * __restrict acc = p->m_acc.get ();
    vec4 * __restrict vel = p->m_vel.get ();
    vec4 * __restrict pos = p->m_pos.get ();

    const unsigned int endId = p->m_countAlive;
    for (size_t i = 0; i < endId; ++i)
    {
      acc [i] += globalA;
      vel [i] += localDT * acc [i];
      pos [i] += localDT * vel [i];
    }
  }

  void FloorUpdater::update (double dt, ParticleData *p)
  {
    const float localDT = (float) dt;

    vec4 * __restrict acc = p->m_acc.get ();
    vec4 * __restrict vel = p->m_vel.get ();
    vec4 * __restrict pos = p->m_pos.get ();

    const size_t endId = p->m_countAlive;
    for (size_t i = 0; i < endId; ++i)
    {
      if (pos [i].y < m_floorY)
      {
        vec4 force = p->m_acc [i];
        float normalFactor = glm::dot (force, vec4 (0.0f, 1.0f, 0.0f, 0.0f));
        if (normalFactor < 0.0f)
          force -= vec4 (0.0f, 1.0f, 0.0f, 0.0f) * normalFactor;

        float velFactor = glm::dot (vel [i], vec4 (0.0f, 1.0f, 0.0f, 0.0f));
        //if (velFactor < 0.0)
        vel [i] -= vec4 (0.0f, 1.0f, 0.0f, 0.0f) * (1.0f + m_bounceFactor) * velFactor;

        acc [i] = force;
      }
    }

  }

  void AttractorUpdater::update (double dt, ParticleData *p)
  {
    const float localDT = (float) dt;

    vec4 * __restrict acc = p->m_acc.get ();
    vec4 * __restrict vel = p->m_vel.get ();
    vec4 * __restrict pos = p->m_pos.get ();

    const size_t endId = p->m_countAlive;
    const size_t countAttractors = m_attractors.size ();
    vec4 off;
    float dist;
    size_t a;
    for (size_t i = 0; i < endId; ++i)
    {
      for (a = 0; a < countAttractors; ++a)
      {
        off.x = m_attractors [a].x - pos [i].x;
        off.y = m_attractors [a].y - pos [i].y;
        off.z = m_attractors [a].z - pos [i].z;
        dist = glm::dot (off, off);

        //if (fabs(dist) > 0.00001)
        dist = m_attractors [a].w / dist;

        acc [i] += off * dist;
      }
    }
  }

  void BasicColorUpdater::update (double dt, ParticleData *p)
  {
    vec4 * __restrict col = p->m_col.get ();
    vec4 * __restrict startCol = p->m_startCol.get ();
    vec4 * __restrict endCol = p->m_endCol.get ();
    vec4 * __restrict t = p->m_time.get ();

    const size_t endId = p->m_countAlive;

	for (size_t i = 0; i < endId; ++i)
	{
		col[i] = glm::mix(startCol[i], endCol[i], t[i].z);
	}
  }

  void PosColorUpdater::update (double dt, ParticleData *p)
  {
    vec4 * __restrict col = p->m_col.get ();
    vec4 * __restrict startCol = p->m_startCol.get ();
    vec4 * __restrict endCol = p->m_endCol.get ();
    vec4 * __restrict t = p->m_time.get ();
    vec4 * __restrict pos = p->m_pos.get ();

    const int endId = (int) p->m_countAlive;
    float scaler, scaleg, scaleb;
    float diffr = m_maxPos.x - m_minPos.x;
    float diffg = m_maxPos.y - m_minPos.y;
    float diffb = m_maxPos.z - m_minPos.z;

    for (int i = 0; i < endId; ++i)
    {
      scaler = (pos [i].x - m_minPos.x) / diffr;
      scaleg = (pos [i].y - m_minPos.y) / diffg;
      scaleb = (pos [i].z - m_minPos.z) / diffb;
      col [i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
      col [i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
      col [i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
      col [i].a = glm::mix (startCol [i].a, endCol [i].a, t [i].z * AUDIOSYSTEM->input.peaklevel[0]);
    }
  }

  void VelColorUpdater::update (double dt, ParticleData *p)
  {
    vec4 * __restrict col = p->m_col.get ();
    vec4 * __restrict startCol = p->m_startCol.get ();
    vec4 * __restrict endCol = p->m_endCol.get ();
    vec4 * __restrict t = p->m_time.get ();
    vec4 * __restrict vel = p->m_vel.get ();

    const size_t endId = p->m_countAlive;
    float scaler, scaleg, scaleb;
    float diffr = m_maxVel.x - m_minVel.x;
    float diffg = m_maxVel.y - m_minVel.y;
    float diffb = m_maxVel.z - m_minVel.z;
    for (size_t i = 0; i < endId; ++i)
    {
      scaler = (vel [i].x - m_minVel.x) / diffr;
      scaleg = (vel [i].y - m_minVel.y) / diffg;
      scaleb = (vel [i].z - m_minVel.z) / diffb;
      col [i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
      col [i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
      col [i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
      col [i].a = glm::mix (startCol [i].a, endCol [i].a, t [i].z);
    }
  }

  void BasicTimeUpdater::update (double dt, ParticleData *p)
  {
    unsigned int endId = p->m_countAlive;
    const float localDT = (float) dt;

    vec4 * __restrict t = p->m_time.get ();

    if (endId == 0) return;

    for (size_t i = 0; i < endId; ++i)
    {
      t [i].x -= localDT;
      // interpolation: from 0 (start of life) till 1 (end of life)
      t [i].z = (float)1.0 - (t [i].x*t [i].w); // .w is 1.0/max life time		

      if (t [i].x < (float)0.0)
      {
        p->kill (i);
        endId = p->m_countAlive < p->m_count ? p->m_countAlive : p->m_count;
      }
    }
  }
}