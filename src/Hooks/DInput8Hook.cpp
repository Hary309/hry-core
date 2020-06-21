#include "DInput8Hook.hpp"

#define CINTERFACE

#include <memory>
#include <windows.h>
#include <cstdio>

#include <dinput.h>

#include "Utils.hpp"
#include "Memory/Hooking.hpp"

namespace hry::hooks
{

using DirectInput8Create_t = typeof(DirectInput8Create);
using DirectInput8_GetDeviceData_t = typeof(IDirectInputDevice8AVtbl::GetDeviceData);

static DirectInput8_GetDeviceData_t oDirectInputGetDeviceData;

static IDirectInput8AVtbl* DIVTable;
static IDirectInputDevice8AVtbl* DIDeviceVTable;

HRESULT __stdcall DirectInputDevice_GetDeviceData(IDirectInputDevice8* self, DWORD cbObjectData, DIDEVICEOBJECTDATA* rgdod, DWORD* pdwInOut, DWORD dwFlags)
{
    return oDirectInputGetDeviceData(self, cbObjectData, rgdod, pdwInOut, dwFlags);
}

bool DInput8Hook::install()
{
    HMODULE libDInput = ::GetModuleHandle(HRY_TEXT("dinput8.dll"));

    if (libDInput == nullptr)
    {
        return false;
    }

    auto dInput8Create = (DirectInput8Create_t*)::GetProcAddress(libDInput, "DirectInput8Create");
    if (dInput8Create == nullptr)
    {
        return false;
    }

    IDirectInput8* DI = nullptr;
    IDirectInputDevice8* DIMouse = nullptr;

    HRESULT hr;

    hr = dInput8Create(::GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DI, nullptr);

    if (FAILED(hr))
    {
        return false;
    }

    DIVTable = DI->lpVtbl;
    hr = DI->lpVtbl->CreateDevice(DI, GUID_SysMouse, &DIMouse, nullptr);

    if (FAILED(hr))
    {
        DI->lpVtbl->Release(DI);
        return false;
    }

    DIDeviceVTable = DIMouse->lpVtbl;

    printf("Hooking GetDeviceData...\n");
    oDirectInputGetDeviceData = memory::HookVTableField(&DIDeviceVTable->GetDeviceData, &DirectInputDevice_GetDeviceData);

    DIMouse->lpVtbl->Release(DIMouse);
    DI->lpVtbl->Release(DI);

    return true;
}

void DInput8Hook::uninstall()
{
    if (DIDeviceVTable != nullptr)
    {
        if (oDirectInputGetDeviceData != nullptr)
        {
            printf("Restoring GetDeviceData...\n");
            memory::HookVTableField(&DIDeviceVTable->GetDeviceData, oDirectInputGetDeviceData);
        }
    }
}

}
