#include <Windows.h>

#include <MinHook.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            MH_Initialize();
        } break;
        case DLL_PROCESS_DETACH:
        {
            MH_Uninitialize();
        } break;
    }

    return TRUE;
}
