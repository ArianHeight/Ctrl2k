#pragma once

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <ctime>
#include <map>
#include <memory> //for shared_ptr and weak_ptr

#define GLEW_STATIC
#include "../Include/GL/glew.h"
#ifdef _WIN32
#include "../Include/GL/wglew.h"
#endif

#include "../Include/GL/freeglut.h"
#include "../Dependencies/SDL2-2.0.7/include/SDL.h"
#include "../Dependencies/SDL2_mixer-2.0.2/include/SDL_mixer.h"

#include "../Dependencies/SOIL for opengl 3.0 context and above/src/SOIL.h"

#define GLM_FORCE_RADIANS
#include "../Include/glm/glm.hpp"
#include "../Include/glm/gtc/type_ptr.hpp"
#include "../Include/glm/gtx/transform.hpp"
#include "../Include/glm/gtx/quaternion.hpp"

//game engine specific
#include "../Include/Classes/Enums.h"
#include "../Include/Classes/Physics/Spawner.h"

#include "../Include/Classes/Render/ScreenBuffer.h"
#include "../Include/Classes/Render/GameWindow.h"

#include "../Include/Classes/Entity/Component.h"
#include "../Include/Classes/Entity/GameObject.h"

#include "../Include/Classes/Entity/Action.h"
#include "../Include/Classes/Entity/FSMState.h"
#include "../Include/Classes/Entity/FSM.h"
#include "../Include/Classes/Entity/AIModule.h"
#include "../Include/Classes/Entity/PathFinding.h"

#include "../Include/Classes/Audio/SoundTrack.h"
#include "../Include/Classes/Audio/SFX.h"
#include "../Include/Classes/Audio/BGM.h"
#include "../Include/Classes/System/AudioManager.h"
#include "../Include/Classes/Audio/AudioEngine.h"

#include "../Include/Classes/Render/Shader.h"
#include "../Include/Classes/Render/Texture.h"
#include "../Include/Classes/System/TextureManager.h"
#include "../Include/Classes/Render/AniFrame.h"
#include "../Include/Classes/Render/Animation.h"
#include "../Include/Classes/System/AnimationManager.h"
#include "../Include/Classes/Render/CollisionMap.h"
#include "../Include/Classes/Render/Map.h"
#include "../Include/Classes/System/MapManager.h"
#include "../Include/Classes/Render/TBuffer.h"
#include "../Include/Classes/Render/TileObject.h"
#include "../Include/Classes/Render/Particle.h"
#include "../Include/Classes/Render/ParticleEngine.h"

#include "../Include/Classes/System/Controller.h"
#include "../Include/Classes/Physics/PhysObject.h"

#include "../Include/Classes/Entity/Entity.h"
#include "../Include/Classes/Entity/Player.h"
#include "../Include/Classes/Entity/NonMovingShooter.h"
#include "../Include/Classes/Render/Camera.h"

#include "../Include/Classes/Physics/DamageArea.h"
#include "../Include/Classes/Physics/PhysEvent.h"
#include "../Include/Classes/Physics/PhysEngine.h"

#include "../Include/Classes/System/FontChar.h"
#include "../Include/Classes/System/Font.h"
#include "../Include/Classes/System/FontManager.h"

#include "../Include/Classes/System/Widgets/Widget.h"
#include "../Include/Classes/System/Widgets/Text.h"
#include "../Include/Classes/System/Widgets/Button.h"
#include "../Include/Classes/System/Widgets/Slider.h"
#include "../Include/Classes/System/Widgets/StatusBar.h"
#include "../Include/Classes/System/Widgets/ItemSlot.h"
#include "../Include/Classes/System/GUI/GUI.h"
#include "../Include/Classes/System/GUI/HUD.h"

#include "../Include/Classes/System/Token.h"
#include "../Include/Classes/System/Lexer.h"
#include "../Include/Classes/System/Parser.h"
#include "../Include/Classes/System/SaveState.h"
#include "../Include/Classes/System/ProgramThread.h"
#include "../Include/Classes/System/ThreadManager.h"

#include "../Include/Classes/System/GameEvent.h"
#include "../Include/Classes/System/Assets.h"
#include "../Include/Classes/System/Game.h"
#include "../Include/Classes/Render/RenderEngine.h"
#include "../Include/Classes/System/MainEngine.h"