#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>
#include "Core/Monument/basictypes.h"

namespace plc
{

typedef std::unordered_map<std::string, std::string> EngineModuleConfig;

class EngineModule : public uncopyable
{
public:
    EngineModule(const EngineModuleConfig& config) {}

    virtual void update() = 0;
};

class AssetModule : public EngineModule
{
protected:

public:
    AssetModule(const EngineModuleConfig& config) : EngineModule(config) {}
};

class PhysicsModule : public EngineModule
{
protected:
    std::weak_ptr<AssetModule> m_assetModuleLink;

public:
    PhysicsModule(const EngineModuleConfig& config) : EngineModule(config) {}
};

class RenderModule : public EngineModule
{
protected:
    std::weak_ptr<AssetModule> m_assetModuleLink;

public:
    RenderModule(const EngineModuleConfig& config) : EngineModule(config) {}
};

class AudioModule : public EngineModule
{
protected:
    std::weak_ptr<AssetModule> m_assetModuleLink;

public:
    AudioModule(const EngineModuleConfig& config) : EngineModule(config) {}
};

class GameModule : public EngineModule
{
protected:
    std::weak_ptr<PhysicsModule> m_physicsModuleLink;
    std::weak_ptr<RenderModule> m_renderModuleLink;
    std::weak_ptr<AudioModule> m_audioModuleLink;
    std::weak_ptr<AssetModule> m_assetModuleLink;

public:
    GameModule(const EngineModuleConfig& config) : EngineModule(config) {}

    inline void link(std::weak_ptr<PhysicsModule> ptr) { m_physicsModuleLink = ptr; }
    inline void link(std::weak_ptr<RenderModule> ptr) { m_renderModuleLink = ptr; }
    inline void link(std::weak_ptr<AudioModule> ptr) { m_audioModuleLink = ptr; }
    inline void link(std::weak_ptr<AssetModule> ptr) { m_assetModuleLink = ptr; }
};

template<class Module> requires std::is_base_of_v<EngineModule, Module>
class EngineModuleSlot : public Uncopyable
{
protected:
    std::shared_ptr<Module> m_module;

public:
    EngineModuleSlot() = default;
    EngineModuleSlot(const EngineModuleConfig& config) : m_module(std::make_shared<Module>(config)) {}

    inline void restart(const EngineModuleConfig& config) { m_module.reset(); m_module = std::make_shared<Module>(config); }

    inline void update() { m_module->update(); }

    inline std::weak_ptr<Module> getLink() { return m_module; }
    inline std::shared_ptr<Module> get() { return m_module; }
};

}