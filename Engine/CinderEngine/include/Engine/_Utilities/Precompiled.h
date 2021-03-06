//////////////////////////////////////////////////////////////////////////
// C LIBRARIES
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cstring>
#include <stddef.h>
#include <cstdio>
#include <cstdarg>

//////////////////////////////////////////////////////////////////////////
// STL
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <map>
#include <utility>
#include <hash_map>
#include <initializer_list>
#include <set>
#include <queue>

//////////////////////////////////////////////////////////////////////////
// I / O
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////
// MEMORY
//////////////////////////////////////////////////////////////////////////

#include <memory>

//////////////////////////////////////////////////////////////////////////
// HELPERS
//////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <locale>
#include <sstream>
#include <thread>

//////////////////////////////////////////////////////////////////////////
// ZILCH
//////////////////////////////////////////////////////////////////////////

using std::string;
#include "Zilch.hpp"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// WINDOWS
//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

#include <WinSock2.h>
#include <windows.h>
#include <windowsx.h>

#endif // _WIN32

//////////////////////////////////////////////////////////////////////////
// GRAPHICS API
//////////////////////////////////////////////////////////////////////////

#include "glew.h"
#include "glfw3.h"
#include "SOIL.h"
#include "CinderMath.h"

//////////////////////////////////////////////////////////////////////////
// ENGINE
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// HELPERS
//////////////////////////////////////////////////////////////////////////

#include "CinderMath.h"
#include "Console.h"
#include "DebugCode.h"
#include "Helpers.h"
#include "WinMain.h"
#include "PhysicsLibrary.h"

#include "DataNode.h"
#include "JSONSerializer.h"

#include "EventSystem.h"
#include "Events.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "AudioEvents.h"
#include "MouseEvent.h"
#include "GameEvent.h"
#include "CollisionEvent.h"
#include "PingEvent.h"
#include "WindowFocusEvent.h"

//////////////////////////////////////////////////////////////////////////
// BASE CLASSES
//////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "Core.h"

//////////////////////////////////////////////////////////////////////////
// OBJECTS
//////////////////////////////////////////////////////////////////////////

// GRAPHICS
#include "Color.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "FrameBufferObject.h"
#include "ShaderStorageBufferObject.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "SpriteSheet.h"

#include "Particles.h"
#include "ParticleRenderer.h"
#include "ParticleGenerators.h"
#include "ParticleUpdaters.h"
#include "GLParticleRenderer.h"

#include "Rect.h"
#include "ShapeData.h"
#include "ShapeGenerator.h"
#include "VertexData.h"
#include "TimeQuery.h"

// PHYSICS
#include "TDLib.h"
#include "Grid2D.h"

// PROCEDURAL
#include "TerrainCreator.h"
#include "TerrainCreator3D.h"
#include "FractalNoise.h"

// AUDIO
#include "Sound.h"

// RESOURCES
#include "ResourceManager.h"
#include "Level.h"
#include "FluidSolver.h"

//////////////////////////////////////////////////////////////////////////
// COMPONENTS
//////////////////////////////////////////////////////////////////////////

#include "Component.h"

// GRAPHICS
#include "IGraphicsObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Tree2D.h"
#include "Terrain2D.h"
#include "Terrain3D.h"
#include "PointLight.h"
#include "GUIText.h"
#include "CinderEngine_UI.h"
#include "FluidBody.h"
#include "Effect.h"
#include "PlayerEffect.h"
#include "VolcanoEffect.h"
#include "TunnelEffect.h"
#include "ExplosionEffect.h"
#include "AttractorEffect.h"
#include "CLParticleRenderer.h"
// PHYSICS
#include "Collider2D.h"
#include "RigidBody2D.h"
#include "CharacterController.h"
// OBJECT

// AUDIO
#include "AudioComponent.h"
#include "Microphone.h"

// GAMEPLAY
#include "CameraShake.h"
#include "EndEventListener.h"
#include "EndTrigger.h"
#include "FirePointController.h"
#include "Follow.h"
#include "Health.h"
#include "InterpolateBackground.h"
#include "LevelTimer.h"
#include "MicahPC.h"
#include "PlayerStats.h"
#include "Rotator.h"
#include "SpriteColorUpdate.h"
#include "FireStarter.h"
#include "FireSystem.h"
#include "WorldStateManager.h"

// SCRIPTING
#include "ScriptComponent.h"

//////////////////////////////////////////////////////////////////////////
// SUB SYSTEMS
//////////////////////////////////////////////////////////////////////////

#include "Pipeline.h"
#include "GridManager.h"
#include "Smoke_Grid.h"

//////////////////////////////////////////////////////////////////////////
// MAIN SYSTEMS
//////////////////////////////////////////////////////////////////////////

#include "BaseSystem.h"
#include "FluidDynamics.h"
#include "AudioSystem.h"
#include "ObjectSystem.h"
#include "UISystem.h"
#include "PhysicsSystem.h"
#include "WindowSystem.h"
#include "UISystem.h"
#include "SceneManager.h"
#include "Thermodynamics.h"
#include "ZilchCompiledLib.h"

#include "PanelManager.h"

#include "FrameRateManager.h"

//////////////////////////////////////////////////////////////////////////\

#pragma hdrstop