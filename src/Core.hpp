#pragma once

#include <scssdk_telemetry.h>
#include <windows.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/GameType.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Utils/Timer.hpp"
#include "Hry/Utils/Utils.hpp"
#include "Hry/Version.hpp"

#include "Config/ConfigManager.hpp"
#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "Renderer/Renderer.hpp"
#include "UI/LoggerWindow.hpp"
#include "UI/MainWindow.hpp"

#include "ImGuiImplEvents.hpp"

struct scs_telemetry_init_params_v100_t;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Event;

struct CoreConfig
{
    bool showLogWindow;
    bool showImGuiDemo;
    float windowOpacity;
};

class Core
{
public:
    inline static HINSTANCE hInstance;
    inline static std::unique_ptr<Logger> Logger;
    inline static GameType GameType;

private:
    bool _isInited = false;

    scs_telemetry_init_params_v100_t* _scsTelemetry = nullptr;

    EventManager _eventMgr;
    InternalEventHandler _eventHandler;
    Renderer _renderer;
    ConfigManager _configMgr;
    KeyBindsManager _keyBindsMgr;
    ModuleManager _moduleMgr;

    MainWindow _mainWindow;
    LoggerWindow _loggerWindow;

    ImGuiImplEvents _imguiImplEvents;

    HryPtr<Config> _coreConfig;
    HryPtr<KeyBinds> _coreKeyBinds;

    bool _showImGuiDemo{};

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);
    // after renderer and imgui is initalized
    void lateInit();

    void imguiRender();

    void initConfig();
    void initKeyBinds();

    [[nodiscard]] bool isInited() const { return _isInited; }

private:
    void onConfigChangesApplied(const ConfigCallbackData& data);

private:
    static bool InstallHooks();
    static void UninstallHooks();

    static hry::GameType DetermineGameType(const char* gameID);
};

HRY_NS_END
