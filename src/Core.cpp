#include "Core.hpp"
#include "scssdk.h"
#include "scssdk_telemetry_event.h"

#include <cstdio>

#include <scssdk_telemetry.h>

#include <Hooks/D3D11Hook.hpp>
#include <Hooks/DInput8Hook.hpp>
#include <stdio.h>

namespace hry
{

Core::Core(HINSTANCE hInst)
    : _renderer(*this)
{
    hInstance = hInst;
}

Core::~Core()
{
    Core::uninstallHooks();
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry) 
{
    _scsTelemetry = scsTelemetry;

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stdin);

	bool success = true;

    success &= scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_frame_start, Core::TelemetryFrameEnd, this) == SCS_RESULT_ok;
    _deltaTime.reset();

    if (success == false)
	{
		printf("Unable to register event callbacks");
		return false;
	}

	success &= Core::installHooks();

    _renderer.init();

    return success;
}

void Core::update() 
{
    float deltaTime = _deltaTime.asSeconds();
    _deltaTime.reset();

    printf("delta: %f\n", deltaTime);
}

void Core::imguiRender() 
{
    
}

void Core::event(const Event& event) 
{

}

bool Core::installHooks()
{
    bool success = true;

    success &= hooks::D3D11Hook::install();
    success &= hooks::DInput8Hook::install();

    if (success)
    {
        printf("Hooks installed!\n");
    }
    else
    {
        printf("Cannot install hooks!\n");
    }

    return success;
}

void Core::uninstallHooks()
{
    hooks::D3D11Hook::uninstall();
    hooks::DInput8Hook::uninstall();
}

void Core::TelemetryFrameEnd(const scs_event_t event, const void* const, const scs_context_t self) 
{
    Core* core = reinterpret_cast<Core*>(self);

    core->update();
}

}
