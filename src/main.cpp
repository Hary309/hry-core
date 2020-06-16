#include <Windows.h>
#include <memory>

#include <MinHook.h>

#include <scssdk_telemetry.h>

#include "Core.hpp"

std::unique_ptr<hry::Core> core;

__declspec(dllexport) SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t *const params)
{
    return SCS_RESULT_ok;
}

__declspec(dllexport) SCSAPI_VOID scs_telemetry_shutdown(void)
{
    core.reset();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            MH_Initialize();
            core = std::make_unique<hry::Core>(hinstDLL);
        } break;
        case DLL_PROCESS_DETACH:
        {
            core.reset();
            MH_Uninitialize();
        } break;
    }

    return TRUE;
}
