#pragma once

#include <windows.h>

#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Utils/Timer.hpp"
#include "Hry/Logger/LoggerCore.hpp"
#include "Hry/Logger/ModuleLogger.hpp"

#include "ImGuiImplEvents.hpp"
#include "Renderer/Renderer.hpp"
#include "Events/EventManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "UI/MainWindow.hpp"


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

    logger::LoggerCore _loggerCore;
    
    renderer::Renderer _renderer;
    events::EventManager _eventMgr;
    key_binding::KeyBindsManager _keyBindsMgr;
    modules::ModuleManager _moduleMgr;
    ui::MainWindow _mainWindow;

    ImGuiImplEvents _imguiImplEvents;

    key_binding::KeyBinds* _coreKeyBinds;

public:
    inline static std::unique_ptr<logger::ModuleLogger> Logger;

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);
    // after renderer and imgui is initalized
    void lateInit();

    void setupKeyBinds();

    void imguiRender();

private:
    static bool InstallHooks();
    static void UninstallHooks();
};

}
