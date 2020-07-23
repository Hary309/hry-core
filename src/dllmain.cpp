#include <memory>

#include <MinHook.h>
#include <Windows.h>
#include <scssdk_telemetry.h>

#include "Core.hpp"

static std::unique_ptr<hry::Core> core;

__declspec(dllexport) SCSAPI_RESULT
    scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
{
    if (version != SCS_TELEMETRY_VERSION_1_01)
    {
        return SCS_RESULT_unsupported;
    }

    if (core->init((scs_telemetry_init_params_v100_t*)params))
    {
        return SCS_RESULT_ok;
    }

    return SCS_RESULT_generic_error;
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
        }
        break;
        case DLL_PROCESS_DETACH:
        {
            core.reset();
            MH_Uninitialize();
        }
        break;
    }

    return TRUE;
}
