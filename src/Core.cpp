#include "Core.hpp"

#include <cstdio>

#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Logger/LoggerFactory.hpp"
#include "Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

Core::Core(HINSTANCE hInst)
    : _renderer(*this), _keyBindsMgr(_eventMgr),
      _moduleMgr("plugins\\hry_plugins", _eventMgr, _keyBindsMgr),
      _mainWindow(_moduleMgr, _keyBindsMgr, _eventMgr), _imguiImplEvents(_eventMgr)
{
    hInstance = hInst;
}

Core::~Core()
{
    Logger->info("Deinitializing...");
    _moduleMgr.unloadAll();
    Core::UninstallHooks();
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    _scsTelemetry = scsTelemetry;

#ifdef DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stdin);
#endif

    LoggerFactory::Init("hry_core.log");
    Logger = LoggerFactory::GetLogger("core");

    Logger->info("Initializing core...");

    bool success = true;

    success &= Core::InstallHooks();

    _renderer.init();
    _eventMgr.init(scsTelemetry);

    _isInited = true;

    return success;
}

void Core::lateInit()
{
    EnableImGui(false);
    initKeyBinds();

    _moduleMgr.init();

    Logger->info("Core successfully initialized!");
}

void Core::initKeyBinds()
{
    _coreKeyBinds = _keyBindsMgr.createKeyBinds("Core");
    _mainWindow.initKeyBinds(*_coreKeyBinds);
    _keyBindsMgr.loadFor(_coreKeyBinds.get());
}

void Core::imguiRender()
{
    ImGui::ShowDemoWindow();
    _mainWindow.renderImGui();

    _eventMgr.imguiRenderSignal.call();
}

bool Core::InstallHooks()
{
    Logger->info("Installing hooks...");

    bool success = true;

    success &= D3D11Hook::Install();
    success &= DInput8Hook::Install();

    if (success)
    {
        Logger->info("Hooks installed");
    }
    else
    {
        Logger->error("Cannot install hooks");
    }

    return success;
}

void Core::UninstallHooks()
{
    Logger->info("Uninstalling hooks...");

    D3D11Hook::Uninstall();
    DInput8Hook::Uninstall();

    Logger->info("Hooks uninstalled");
}

HRY_NS_END
