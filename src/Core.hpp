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

    LoggerCore _loggerCore;
    
    Renderer _renderer;
    EventManager _eventMgr;
    KeyBindsManager _keyBindsMgr;
    ModuleManager _moduleMgr;
    MainWindow _mainWindow;

    ImGuiImplEvents _imguiImplEvents;

    KeyBinds* _coreKeyBinds;

public:
    inline static std::unique_ptr<ModuleLogger> Logger;

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
