#pragma once

#include <scssdk_telemetry.h>
#include <windows.h>

#include "Hry/Events/Event.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Utils.hpp"
#include "Hry/Utils/Timer.hpp"

#include "Config/ConfigManager.hpp"
#include "Events/EventManager.hpp"
#include "KeyBinding/KeyBindsManager.hpp"
#include "Modules/ModuleManager.hpp"
#include "Renderer/Renderer.hpp"
#include "UI/MainWindow.hpp"

#include "ImGuiImplEvents.hpp"

struct scs_telemetry_init_params_v100_t;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Event;

class Core
{
public:
    static inline HINSTANCE hInstance;

private:
    bool _isInited = false;

    scs_telemetry_init_params_v100_t* _scsTelemetry = nullptr;

    Renderer _renderer;
    EventManager _eventMgr;
    ConfigManager _configMgr;
    KeyBindsManager _keyBindsMgr;
    ModuleManager _moduleMgr;
    MainWindow _mainWindow;

    ImGuiImplEvents _imguiImplEvents;

    DelegateDeleterUniquePtr_t<Config> _coreConfig;
    DelegateDeleterUniquePtr_t<KeyBinds> _coreKeyBinds;

public:
    inline static std::unique_ptr<Logger> Logger;

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
    static bool InstallHooks();
    static void UninstallHooks();
};

HRY_NS_END
