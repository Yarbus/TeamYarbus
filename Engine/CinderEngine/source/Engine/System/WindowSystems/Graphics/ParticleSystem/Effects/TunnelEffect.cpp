/******************************************************************************/
/*!
\file   TunnelEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "glew.h"
#include "EditorUI.h"
#include "TunnelEffect.h"
#include "ResourceManager.h"

namespace Framework
{
  bool TunnelEffect::initialize (size_t numParticles)
  {
    texture = Resources::RS->Get_Texture ("Particle1.png");
    shader = Resources::RS->Get_Shader ("Particle");
    //
    // particles
    //
    const size_t NUM_PARTICLES = numParticles == 0 ? 10000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);

    //
    // emitter:
    //
    auto particleEmitter = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter->m_emitRate = (float) NUM_PARTICLES*0.45f;

      // pos:
      m_posGenerator = std::make_shared<RoundPosGen> ();
      m_posGenerator->m_center = vec4{ 0.0, 0.0, 0.0, 0.0 };
      m_posGenerator->m_radX = 0.15f;
      m_posGenerator->m_radY = 0.15f;
      particleEmitter->addGenerator (m_posGenerator);

      m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = vec4{ 0.7, 0.0, 0.7, 1.0 };
      m_colGenerator->m_maxStartCol = vec4{ 1.0, 1.0, 1.0, 1.0 };
      m_colGenerator->m_minEndCol = vec4{ 0.5, 0.0, 0.6, 0.0 };
      m_colGenerator->m_maxEndCol = vec4{ 0.7, 0.5, 1.0, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      auto velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = vec4{ 0.0f, 0.0f, 0.15f, 0.0f };
      velGenerator->m_maxStartVel = vec4{ 0.0f, 0.0f, 0.45f, 0.0f };
      particleEmitter->addGenerator (velGenerator);

      auto timeGenerator = std::make_shared<BasicTimeGen> ();
      timeGenerator->m_minTime = 1.0;
      timeGenerator->m_maxTime = 3.5;
      particleEmitter->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter);

    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater> ();
    //colorUpdater->m_minPos = vec4{ -1.0f };
    //colorUpdater->m_maxPos = vec4{ 1.0f };
    m_system->addUpdater (colorUpdater);

    auto eulerUpdater = std::make_shared<EulerUpdater> ();
    eulerUpdater->m_globalAcceleration = vec4{ 0.0, 0.0, 0.0, 0.0 };
    m_system->addUpdater (eulerUpdater);

    return true;
  }

  bool TunnelEffect::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void TunnelEffect::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }

  void TunnelEffect::addUI (TwBar* tw)
  {
    Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
  }

  void TunnelEffect::removeUI (TwBar* tw)
  {
    Editor::RemoveVar (tw, "start col min");
    Editor::RemoveVar (tw, "start col max");
    Editor::RemoveVar (tw, "end col min");
    Editor::RemoveVar (tw, "end col max");
  }

  void TunnelEffect::update (double dt)
  {
    static double time = 0.0;
    time += dt;

    m_posGenerator->m_center.x = 0.1f*sin ((float) time*2.5f);
    m_posGenerator->m_center.y = 0.1f*cos ((float) time*2.5f);
    m_posGenerator->m_radX = 0.15f + 0.05f*sin ((float) time);
    m_posGenerator->m_radY = 0.15f + 0.05f*sin ((float) time)*cosf ((float) time*0.5f);
  }

  void TunnelEffect::cpuUpdate (double dt)
  {
    m_system->update (dt);
  }

  void TunnelEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void TunnelEffect::render ()
  {
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture->Get_ID ());
    shader->uni1f ("size", 2.0f);
    m_renderer->render ();
  }

  void TunnelEffect::Initialize ()
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void TunnelEffect::Serialize (Serializer::DataNode* data)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

}