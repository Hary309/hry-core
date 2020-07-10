#include "DInput8Hook.hpp"

#define COBJMACROS
#define CINTERFACE

#include <memory>
#include <windows.h>
#include <cstdio>

#include <dinput.h>

#include "Utils.hpp"
#include "Memory/Hooking.hpp"

namespace hry::hooks
{

using DirectInput8Create_t = decltype(DirectInput8Create);
using DirectInput8_GetDeviceData_t = decltype(IDirectInputDevice8AVtbl::GetDeviceData);

static DirectInput8_GetDeviceData_t oDirectInputGetDeviceData;

static IDirectInput8AVtbl* DIVTable;
static IDirectInputDevice8AVtbl* DIDeviceVTable;

HRESULT __stdcall new_DirectInputDevice_GetDeviceData(IDirectInputDevice8A* self, DWORD cbObjectData, DIDEVICEOBJECTDATA* rgdod, DWORD* pdwInOut, DWORD dwFlags)
{
    auto result = oDirectInputGetDeviceData(self, cbObjectData, rgdod, pdwInOut, dwFlags);

    if (self && rgdod && pdwInOut)
    {
        DInput8Hook::OnGetDeviceData.call(self, { rgdod, rgdod + (*pdwInOut) });
    }

    return result;
}

bool DInput8Hook::Install()
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
    hr = IDirectInput8_CreateDevice(DI, GUID_SysMouse, &DIMouse, nullptr);

    if (FAILED(hr))
    {
        IDirectInput8_Release(DI);
        return false;
    }

    DIDeviceVTable = DIMouse->lpVtbl;

    printf("Hooking GetDeviceData...\n");
    oDirectInputGetDeviceData = memory::HookVTableField(&DIDeviceVTable->GetDeviceData, &new_DirectInputDevice_GetDeviceData);

    IDirectInputDevice8_Release(DIMouse);
    IDirectInput8_Release(DI);

    return true;
}

void DInput8Hook::Uninstall()
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
