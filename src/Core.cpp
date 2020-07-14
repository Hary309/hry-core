#include "Core.hpp"

#include <cstdio>

#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Hry/Utils/Signal.hpp"


namespace hry
{

Core::Core(HINSTANCE hInst)
    : 
    _renderer(*this),
    _moduleMgr("plugins\\hry_plugins", _eventMgr, _loggerCore),
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

    success &= scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_frame_start, Core::TelemetryFrameEnd, this) == SCS_RESULT_ok;
    _deltaTime.reset();

    if (success == false)
	{
		Logger->error("Unable to register event callbacks");
		return false;
	}

	success &= Core::InstallHooks();

    _renderer.init();
    _eventMgr.init();
    _moduleMgr.init();

    Logger->info("Core successfully initialized!");

    return success;
}

void Core::update()
{
    [[maybe_unused]] float deltaTime = _deltaTime.asSeconds();
    _deltaTime.reset();

}

void Core::imguiRender() 
{
    ImGui::ShowDemoWindow();
    _mainWindow.renderImGui();
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

void Core::TelemetryFrameEnd(const scs_event_t event, const void* const, const scs_context_t self) 
{
    Core* core = reinterpret_cast<Core*>(self);

    core->update();
}

}
