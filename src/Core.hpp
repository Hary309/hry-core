/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "ImGuiImplEvents.hpp"

#include "AxisBinding/AxisBindsManager.hpp"
#include "Config/ConfigManager.hpp"
#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "Renderer/Renderer.hpp"
#include "SCSSDK/ChannelAggregator.hpp"
#include "UI/LoggerWindow.hpp"
#include "UI/MainWindow.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/GameType.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Utils/Timer.hpp"
#include "Hry/Utils/Utils.hpp"
#include "Hry/Version.hpp"

#include <scssdk_telemetry.h>
#include <windows.h>

namespace hry
{
class Event;

struct CoreConfig
{
    bool showLogWindow;
    bool showImGuiDemo;
    float logWindowOpacity;
};

class Core
{
public:
    inline static HINSTANCE hInstance;
    inline static std::unique_ptr<Logger> Logger;
    inline static GameType GameType;

private:
    bool _isInited = false;

    EventManager _eventMgr;
    InternalEventDispatcher _eventDispatcher;
    ChannelAggregator _channelAggregator;
    Renderer _renderer;
    ConfigManager _configMgr;
    KeyBindsManager _keyBindsMgr;
    AxisBindsManager _axisBindsMgr;
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
    void initAxisBinds();

    [[nodiscard]] bool isInited() const { return _isInited; }

private:
    void onConfigChangesApplied();

private:
    static bool InstallHooks();
    static void UninstallHooks();

    static hry::GameType DetermineGameType(const char* gameID);
};
}
