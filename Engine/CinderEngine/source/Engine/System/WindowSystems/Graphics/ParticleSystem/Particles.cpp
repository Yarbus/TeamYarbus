/******************************************************************************/
/*!
\file   Particles.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  void ParticleData::generate (size_t maxSize)
  {
    m_count = maxSize;
    m_countAlive = 0;

    m_pos.reset (new vec4 [maxSize]);
    m_col.reset (new vec4 [maxSize]);
    m_startCol.reset (new vec4 [maxSize]);
    m_endCol.reset (new vec4 [maxSize]);
    m_vel.reset (new vec4 [maxSize]);
    m_acc.reset (new vec4 [maxSize]);
    m_time.reset (new vec4 [maxSize]);
    m_alive.reset (new bool [maxSize]);
  }

  void ParticleData::kill (size_t id)
  {
    //if (m_countAlive > 0) // maybe this if can be removed?
    {
      m_alive [id] = false;
      swapData (id, m_countAlive - 1);
      --m_countAlive;
    }
  }

  void ParticleData::wake (size_t id)
  {
    //if (m_countAlive < m_count) // maybe this if can be removed?
    {
      m_alive [id] = true;
      //swapData(id, m_countAlive);
      m_countAlive++;
    }
  }

  void ParticleData::swapData (size_t a, size_t b)
  {
    /*std::swap(m_pos[a], m_pos[b]);
    std::swap(m_col[a], m_col[b]);
    std::swap(m_startCol[a], m_startCol[b]);
    std::swap(m_endCol[a], m_endCol[b]);
    std::swap(m_vel[a], m_vel[b]);
    std::swap(m_acc[a], m_acc[b]);
    std::swap(m_time[a], m_time[b]);
    std::swap(m_alive[a], m_alive[b]);*/
    m_pos [a] = m_pos [b];
    m_col [a] = m_col [b];
    m_startCol [a] = m_startCol [b];
    m_endCol [a] = m_endCol [b];
    m_vel [a] = m_vel [b];
    m_acc [a] = m_acc [b];
    m_time [a] = m_time [b];
    //m_alive[a] = m_alive[b];*/
  }

  void ParticleData::copyOnlyAlive (const ParticleData *source, ParticleData *destination)
  {
    assert (source->m_count == destination->m_count);

    size_t id = 0;
    for (size_t i = 0; i < source->m_countAlive; ++i)
    {
      //if (source->m_alive[i])
      {
        destination->m_pos [id] = source->m_pos [i];
        destination->m_col [id] = source->m_col [i];
        destination->m_startCol [id] = source->m_startCol [i];
        destination->m_endCol [id] = source->m_endCol [i];
        destination->m_vel [id] = source->m_vel [i];
        destination->m_acc [id] = source->m_acc [i];
        destination->m_time [id] = source->m_time [i];
        destination->m_alive [id] = true;
        id++;
      }
    }

    destination->m_countAlive = id;
  }

  size_t ParticleData::computeMemoryUsage (const ParticleData &p)
  {
    return p.m_count * (7 * sizeof(vec4) + sizeof(bool)) + sizeof(size_t) * 2;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ParticleEmitter class 

  void ParticleEmitter::emit (double dt, ParticleData *p)
  {
    if (active)
    {
      const size_t maxNewParticles = static_cast<size_t>(0.1f * m_emitRate);
      const size_t startId = p->m_countAlive;
      const size_t endId = std::min (startId + maxNewParticles, p->m_count - 1);

      for (auto &gen : m_generators)
        gen->generate (dt, p, startId, endId);

      for (size_t i = startId; i < endId; ++i)
      {
        p->wake (i);
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ParticleSystem class 

  ////////////////////////////////////////////////////////////////////////////////
  ParticleSystem::ParticleSystem (size_t maxCount)
  {
  }


  void ParticleSystem::init (int maxCount)
  {
    //SSBOParticles = new SSBO (maxCount * (7 * sizeof (vec4) + sizeof(bool) +2 * sizeof(size_t)));
    //std::cout << maxCount * (7 * sizeof (vec4) + sizeof(bool) +2 * sizeof(size_t)) << "\n";
    m_count = maxCount;
    m_particles.generate (maxCount);
    m_aliveParticles.generate (maxCount);

    for (int i = 0; i < maxCount; ++i)
      m_particles.m_alive [i] = false;
  }


  void ParticleSystem::update (double dt)
  {
    for (auto & em : m_emitters)
    {
      em->emit (dt, &m_particles);
    }

    for (auto & up : m_updaters)
    {
      up->update (dt, &m_particles);
    }

    //ParticleData::copyOnlyAlive(&m_particles, &m_aliveParticles);
  }

  void ParticleSystem::reset ()
  {
    m_particles.m_countAlive = 0;
  }

  size_t ParticleSystem::computeMemoryUsage (const ParticleSystem &p)
  {
    return 2 * ParticleData::computeMemoryUsage (p.m_particles);
  }

}