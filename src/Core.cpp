#include "Core.hpp"

#include <cstdio>

#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Utils/ImGuiUtils.hpp"


namespace hry
{

Core::Core(HINSTANCE hInst)
    : 
    _renderer(*this),
    _keyBindsMgr(_eventMgr),
    _moduleMgr("plugins\\hry_plugins", _eventMgr, _keyBindsMgr, _loggerCore),
    _mainWindow(_moduleMgr, _keyBindsMgr),
    _imguiImplEvents(_eventMgr)
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

    _loggerCore.init("hry_core.log");
    Logger = _loggerCore.createModuleLogger("core");

    Logger->info("Initializing core...");

	bool success = true;

	success &= Core::InstallHooks();
    
    _renderer.init();
    _eventMgr.init(scsTelemetry);

    return success;
}

void Core::lateInit() 
{
    setupKeyBinds();

    _moduleMgr.init();

    Logger->info("Core successfully initialized!");
}

void Core::setupKeyBinds() 
{
    _coreKeyBinds = _keyBindsMgr.createKeyBinds("Core");
    _mainWindow.setupKeyBinds(*_coreKeyBinds);
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

    success &= hooks::D3D11Hook::Install();
    success &= hooks::DInput8Hook::Install();

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

    hooks::D3D11Hook::Uninstall();
    hooks::DInput8Hook::Uninstall();

    Logger->info("Hooks uninstalled");
}

}
