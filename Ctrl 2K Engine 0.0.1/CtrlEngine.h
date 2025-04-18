#pragma once
#include "Core/Palace/EngineModule.h"

class CtrlEngine
{
private:
    plc::EngineModuleSlot<plc::AssetModule> m_assetLoader;
    plc::EngineModuleSlot<plc::PhysicsModule> m_physicsEngine;
    plc::EngineModuleSlot<plc::RenderModule> m_renderer;
    plc::EngineModuleSlot<plc::AudioModule> m_audioEngine;
    plc::EngineModuleSlot<plc::GameModule> m_gameData;

public:
    CtrlEngine();
    ~CtrlEngine();
};