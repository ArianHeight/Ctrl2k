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
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <GL/freeglut.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include <SOIL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

//game engine specific
#include <Classes/Enums.h>
#include <Classes/Physics/Spawner.h>

#include <Classes/Render/ScreenBuffer.h>
#include <Classes/Render/GameWindow.h>

#include <Classes/Entity/Component.h>
#include <Classes/Entity/GameObject.h>

#include <Classes/Entity/Action.h>
#include <Classes/Entity/FSMState.h>
#include <Classes/Entity/FSM.h>
#include <Classes/Entity/AIModule.h>
#include <Classes/Entity/PathFinding.h>

#include <Classes/Audio/SoundTrack.h>
#include <Classes/Audio/SFX.h>
#include <Classes/Audio/BGM.h>
#include <Classes/System/AudioManager.h>
#include <Classes/Audio/AudioEngine.h>

#include <Classes/Render/Shader.h>
#include <Classes/Render/Texture.h>
#include <Classes/System/TextureManager.h>
#include <Classes/Render/AniFrame.h>
#include <Classes/Render/Animation.h>
#include <Classes/System/AnimationManager.h>
#include <Classes/Render/CollisionMap.h>
#include <Classes/Render/Map.h>
#include <Classes/System/MapManager.h>
#include <Classes/Render/TBuffer.h>
#include <Classes/Render/TileObject.h>
#include <Classes/Render/Particle.h>
#include <Classes/Render/ParticleEngine.h>

#include <Classes/System/Controller.h>
#include <Classes/Physics/PhysObject.h>

#include <Classes/Entity/Entity.h>
#include <Classes/Entity/Player.h>
#include <Classes/Entity/NonMovingShooter.h>
#include <Classes/Render/Camera.h>

#include <Classes/Physics/DamageArea.h>
#include <Classes/Physics/PhysEvent.h>
#include <Classes/Physics/PhysEngine.h>

#include <Classes/System/FontChar.h>
#include <Classes/System/Font.h>
#include <Classes/System/FontManager.h>

#include <Classes/System/Widgets/Widget.h>
#include <Classes/System/Widgets/Text.h>
#include <Classes/System/Widgets/Button.h>
#include <Classes/System/Widgets/Slider.h>
#include <Classes/System/Widgets/StatusBar.h>
#include <Classes/System/Widgets/ItemSlot.h>
#include <Classes/System/GUI/GUI.h>
#include <Classes/System/GUI/HUD.h>

#include <Classes/System/Token.h>
#include <Classes/System/Lexer.h>
#include <Classes/System/Parser.h>
#include <Classes/System/SaveState.h>
#include <Classes/System/ProgramThread.h>
#include <Classes/System/ThreadManager.h>

#include <Classes/System/GameEvent.h>
#include <Classes/System/Assets.h>
#include <Classes/System/Game.h>
#include <Classes/Render/RenderEngine.h>
#include <Classes/System/MainEngine.h>