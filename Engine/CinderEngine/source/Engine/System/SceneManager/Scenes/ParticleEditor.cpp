/******************************************************************************/
/*!
\file   ParticleEditor.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "ParticleEditor.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "Particles.h"
#include "ParticleUpdaters.h"
#include "particleGenerators.h"
#include "ParticleRenderer.h"
#include "TimeQuery.h"
#include "Effect.h"
#include "AntTweakBar.h"
#include "WindowSystem.h"
#include "EditorUI.h"



namespace Framework
{
  std::string s = "Hello";
   int reverbPreset, oldReverbPreset;
  float LPFcutOff = 6000, LPFresonance = 1;
  float HPFcutOff = 5000, HPFresonance = 1;
  float oldLPFCutOff = LPFcutOff, oldLPFResonance = LPFresonance;
  float oldHPFCutOff = HPFcutOff, oldHPFResonance = HPFresonance;
  bool lpf = false, oldLPF = false, hpf = false, oldHPF = false, reverb = false, oldReverb = false;
  Sound* bg;
  TwBar *myBar;
  TwBar* audioBar;
  std::shared_ptr<IEffect> gEffects [4];
  IEffect *gCurrentEffect = nullptr;
  int gCurrentEffectID = 0;
  int gSelectedEffect = 0;
  int gNumParticles = 0;
  int gNumAlive = 0;
  static float volume = 1.0f;
  static float Fps = 0.0f;
  static double AppTime = 0;

  unsigned int gParticleTexture = 0;
  Shader* mProgram;
  struct CAMERA
  {
    float camDistance;
    glm::vec3 cameraDir;
    glm::mat4 modelviewMatrix;
    glm::mat4 projectionMatrix;
  } camera_;


  Camera camera1 (true);
  CpuTimeQuery cpuParticlesUpdate;
  CpuTimeQuery cpuBuffersUpdate;
  GpuTimerQuery gpuUpdate;
  double gpuUpdateTime = 0;
  GpuTimerQuery gpuRender;
  double gpuRenderTime = 0;

  bool gAnimationOn = true;

  // Constructor
  ParticleEditor::ParticleEditor ()
  {
    TwTerminate ();
  }

  // Destructor
  ParticleEditor::~ParticleEditor ()
  {
    gCurrentEffect->clean ();
    glDeleteTextures (1, &gParticleTexture);
  }


  void ParticleEditor::Key_Pressed(int key, int scanCode, int action, int mods)
  {
    switch (key)
    {
    case GLFW_KEY_0:
      gSelectedEffect = 0;
      break;
    case GLFW_KEY_1:
      gSelectedEffect = 1;
      break;
    case GLFW_KEY_2:
      gSelectedEffect = 2;
      break;
    case GLFW_KEY_W:
      camera1.Zoom (1.0f);
      break;
    default:
      break;
    }
  }


  void ParticleEditor::Mouse_Button(int button, int state, int mods)
  {
    TwEventMouseButtonGLFW (button, state);
  }


  void ParticleEditor::Mouse_Moved(double xPos, double yPos)
  {
    TwEventMousePosGLFW ((int)xPos, (int)yPos);
  }


  void ParticleEditor::Load_Scene (const char* filename)
  {
    //TwInit (TW_OPENGL, NULL);
    // or
#ifdef _USE_ANTWEAK
    TwInit(TW_OPENGL_CORE, NULL); // for core profile
#endif

    Change_Size (WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    //
    // some global GL states
    //
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.f, 0.f, 0.f, 0.0f);
    TwWindowSize (WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    //camera_.cameraDir [0] = 0.0f;
    //camera_.cameraDir [1] = 0.0f;
    //camera_.cameraDir [2] = 1.0f;
    //camera_.camDistance = 0.5f;

    gParticleTexture = Resources::RS->Get_Texture ("Particle2.png")->Get_ID ();
    mProgram = Resources::RS->Get_Shader ("Particle");
    mProgram->Use ();
    mProgram->uni1i ("tex", 0);
    mProgram->Disable ();


    gEffects [0] = EffectFactory::create ("attractors");
    gEffects [0]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [0]->initializeRenderer ();
    gEffects [1] = EffectFactory::create ("fountain");
    gEffects [1]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [1]->initializeRenderer ();
    gEffects [2] = EffectFactory::create ("tunnel");
    gEffects [2]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [2]->initializeRenderer ();
    gEffects [3] = EffectFactory::create ("explosion");
    gEffects [3]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [3]->initializeRenderer ();
    gCurrentEffectID = 0;
    gCurrentEffect = gEffects [0].get ();

    gpuUpdate.init ();
    gpuRender.init ();

    myBar = TwNewBar ("NameOfMyTweakBar");

    audioBar = TwNewBar("Audio");

    TwAddVarRO (myBar, "FPS", TW_TYPE_FLOAT, &Fps, NULL);
    Editor::AddTweak (myBar, "animate", &gAnimationOn, "");
    Editor::AddVar (myBar, "particles", &gNumParticles, "");
    Editor::AddVar (myBar, "alive", &gNumAlive, "");
    Editor::AddVar (myBar, "cpu particles", &cpuParticlesUpdate.m_time, "precision=3 group=timers");
    Editor::AddVar (myBar, "cpu buffers", &cpuBuffersUpdate.m_time, "precision=3 group=timers");
    //Editor::      myBar, 
    Editor::AddVar (myBar, "gpu buffer", &gpuUpdate.getTime (), "precision=3 group=timers");
    Editor::AddVar (myBar, "gpu render", &gpuRender.getTime (), "precision=3 group=timers");
    //Editor::
    //Editor::AddTweakDir3f (myBar, "camera", &camera1.viewDirection.x, "");
    //Editor::AddTweak (myBar, "camera distance", &camera1.fov, "min=0.05 max=4.0 step=0.01");
    Editor::AddSeparator (myBar);
    Editor::AddTweak (myBar, "effect id", &gSelectedEffect, "min=0 max=3");



    gCurrentEffect->addUI (myBar);
    bg = Resources::RS->Get_Sound("music2.mp3");
    bg->Play();
    Editor::AddTweakText(audioBar, "string", &s, "");
    Editor::AddTweak(audioBar, "volume", bg->GetVolumePtr (), "min=0.0 step=0.01 max=1.0");
    Editor::AddTweak(audioBar, "lpf", &lpf, "group=LPF");
    Editor::AddTweak(audioBar, "LPFcutoff", &LPFcutOff, "min=10.0 step=50 max=22000.0 group=LPF");
    Editor::AddTweak(audioBar, "LPFresonance", &LPFresonance, "min=1.0 step=0.5 max=10.0 group=LPF");
    Editor::AddTweak(audioBar, "hpf", &hpf, "group=HPF");
    Editor::AddTweak(audioBar, "HPFcutoff", &HPFcutOff, "min=10.0 step=50 max=22000.0 group=HPF");
    Editor::AddTweak(audioBar, "HPFresonance", &HPFresonance, "min=1.0 step=0.5 max=10.0 group=HPF");
    Editor::AddTweak(audioBar, "reverb", &reverb, "group=REVERB");
    Editor::AddTweak(audioBar, "preset", &reverbPreset, "min=0 max=23 group=REVERB");
    Editor::AddSeparator(audioBar);    

    Camera::main->worldToView = glm::lookAt (Camera::main->viewDirection * 0.5f, Camera::main->position, Camera::main->up);
  }


  void ParticleEditor::Update (const double dt)
  {
    double deltaTime;
    Utils::updateTimer (&deltaTime, &AppTime);
    Utils::calculateFps (&Fps);
    if (!gAnimationOn)
      return;

    if (gSelectedEffect != gCurrentEffectID)
    {
      gCurrentEffect->removeUI (myBar);
      gCurrentEffectID = gSelectedEffect;
      gCurrentEffect = gEffects [gCurrentEffectID].get ();
      gCurrentEffect->addUI (myBar);
    }

    gCurrentEffect->update (deltaTime);

    cpuParticlesUpdate.begin ();
    gCurrentEffect->cpuUpdate (deltaTime);
    gNumAlive = 0;// gParticleSystem->numAliveParticles();
    cpuParticlesUpdate.end ();

    cpuBuffersUpdate.begin ();
    gpuUpdate.begin ();
    gCurrentEffect->gpuUpdate (deltaTime);
    gpuUpdate.end ();
    cpuBuffersUpdate.end ();
    gpuUpdate.updateResults (GpuTimerQuery::WaitOption::WaitForResults);

    gNumParticles = gCurrentEffect->numAllParticles ();
    gNumAlive = gCurrentEffect->numAliveParticles ();

    if (lpf != oldLPF)
    {
      if (lpf)
        bg->LowPassFilter();
      else
        bg->Get_Channel()->removeDSP (bg->Get_LPF ());
      oldLPF = lpf;      
    }
    if (lpf && oldLPFResonance != LPFresonance || oldLPFCutOff != LPFcutOff)
    {
      bg->SetLPF(LPFcutOff, LPFresonance);
      oldLPFCutOff = LPFcutOff;
      oldLPFResonance = LPFresonance;
    }

    if (hpf != oldHPF)
    {
      if (hpf)
        bg->HighPassFilter();
      else
        bg->Get_Channel()->removeDSP(bg->Get_HPF());
      oldHPF = hpf;
    }
    if (hpf && oldHPFResonance != HPFresonance || oldHPFCutOff != HPFcutOff)
    {
      bg->SetHPF(HPFcutOff, HPFresonance);
      oldHPFCutOff = HPFcutOff;
      oldHPFResonance = HPFresonance;
    }

    if (reverb != oldReverb)
    {
      if (reverb)
      {
        bg->Reverb();
        bg->AddReverbPreset(Sound::ReverbPresetName(reverbPreset));
      }
      else
        bg->Get_Channel()->removeDSP(bg->Get_RVRB());
      oldReverb = reverb;
    }
    if (reverb && oldReverbPreset != reverbPreset)
    {
      bg->AddReverbPreset(Sound::ReverbPresetName (reverbPreset));
      oldReverbPreset = reverbPreset;
    }
  }


  void ParticleEditor::Draw ()
  {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable (GL_DEPTH_TEST);

    //
    // camera
    //
    //camera_.modelviewMatrix = glm::lookAt (Camera::main->viewDirection * camera_.camDistance, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 1.0f, 0.0f));

    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, gParticleTexture);

    glEnable (GL_PROGRAM_POINT_SIZE);

    mProgram->Use ();
    mProgram->uniMat4 ("matProjection", glm::value_ptr (Camera::main->GetViewToProjectionMatrix()));
    mProgram->uniMat4 ("matModelview", glm::value_ptr (Camera::main->GetWorldToViewMatrix()));

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    gpuRender.begin ();
    gCurrentEffect->render ();
    gpuRender.end ();

    glDisable (GL_BLEND);

    mProgram->Disable ();

    gpuRender.updateResults (GpuTimerQuery::WaitOption::WaitForResults);

    TwDraw ();
  }

  void ParticleEditor::Change_Size (int w, int h)
  {
    float aspect = (float) w / (float) h;

    // Set the viewport to be the entire window
    glViewport (0, 0, w, h);

    // setup projection matrix
    Camera::main->viewToProjection = glm::perspective (45.0f, aspect, 0.1f, 10.0f);
    TwWindowSize (w, h);
  }


}