#include <Windows.h>
#include <memory>

#include <MinHook.h>

#include "Core.hpp"

std::unique_ptr<hry::Core> core;

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
