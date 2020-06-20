#include "Core.hpp"

#include <cstdio>

#include <scssdk_telemetry.h>

#include <Hooks/D3D11Hook.hpp>

namespace hry
{

Core::Core(HINSTANCE hInst)
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

	success &= Core::installHooks();

    return success;
}

bool Core::installHooks()
{
    bool success = true;

    success &= hooks::D3D11Hook::install();

    return success;
}

void Core::uninstallHooks()
{
    hooks::D3D11Hook::uninstall();
}

}
