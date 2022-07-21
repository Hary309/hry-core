/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Core.hpp"
#include "scssdk.h"

#include "fmt/core.h"

#include <MinHook.h>
#include <Windows.h>
#include <scssdk_telemetry.h>

#include <memory>

namespace
{
std::unique_ptr<hry::Core> core;
HINSTANCE hInstance;
}

__declspec(dllexport) SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
{
    if (version != SCS_TELEMETRY_VERSION_1_01)
    {
        return SCS_RESULT_unsupported;
    }
    MH_Initialize();

    auto* initParams = (scs_telemetry_init_params_v100_t*)(params);

    auto log = initParams->common.log;

    core = std::make_unique<hry::Core>(hInstance);

    if (core->isInited())
    {
        log(SCS_LOG_TYPE_message, "hry_core is already initialized");
        return SCS_RESULT_ok;
    }

    try
    {
        if (core->init(initParams))
        {
            return SCS_RESULT_ok;
        }
    }
    catch (const std::exception& ex)
    {
        log(SCS_LOG_TYPE_error, fmt::format("[hry-core] {}", ex.what()).c_str());
    }

    return SCS_RESULT_generic_error;
}

__declspec(dllexport) SCSAPI_VOID scs_telemetry_shutdown(void)
{
    MH_Uninitialize();
    core.reset();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID /*unused*/)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            hInstance = hinstDLL;
        }
        break;
        case DLL_PROCESS_DETACH:
        {
            core.reset();
        }
        break;
    }

    return TRUE;
}
