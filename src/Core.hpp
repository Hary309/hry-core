#pragma once

#include <windows.h>

#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/Utils/Timer.hpp"
#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/Logger/ModuleLogger.hpp"

#include "ImGuiImplEvents.hpp"
#include "Renderer/Renderer.hpp"
#include "Events/EventManager.hpp"
#include "Modules/ModuleManager.hpp"


struct scs_telemetry_init_params_v100_t;

namespace hry
{

class Event;

class Core
{
public:
    static inline HINSTANCE hInstance;

private:
    scs_telemetry_init_params_v100_t* _scsTelemetry = nullptr;

    renderer::Renderer _renderer;
    events::EventManager _eventMgr;
    logger::LoggerCore _loggerCore;
    modules::ModuleManager _moduleMgr;
    
    utils::Timer _deltaTime;

    ImGuiImplEvents _imguiImplEvents;

public:
    inline static std::unique_ptr<logger::ModuleLogger> Logger;

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);

    void update();
    void imguiRender();

private:
    static bool InstallHooks();
    static void UninstallHooks();

    static void TelemetryFrameEnd(const scs_event_t event, const void* const, const scs_context_t self);
};

}
