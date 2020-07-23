#include "DInput8Hook.hpp"

#define COBJMACROS
#define CINTERFACE

#include <cstdio>
#include <memory>

#include <dinput.h>
#include <windows.h>

#include "Hry/Memory/Hooking.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

using DirectInput8Create_t = decltype(DirectInput8Create);
using DirectInput8_GetDeviceData_t = decltype(IDirectInputDevice8AVtbl::GetDeviceData);

static DirectInput8_GetDeviceData_t oDirectInputGetDeviceData;

static IDirectInput8AVtbl* DIVTable;
static IDirectInputDevice8AVtbl* DIDeviceVTable;

HRESULT __stdcall new_DirectInputDevice_GetDeviceData(
    IDirectInputDevice8A* self,
    DWORD cbObjectData,
    DIDEVICEOBJECTDATA* rgdod,
    DWORD* pdwInOut,
    DWORD dwFlags)
{
    auto result = oDirectInputGetDeviceData(self, cbObjectData, rgdod, pdwInOut, dwFlags);

    if (self && rgdod && pdwInOut)
    {
        DInput8Hook::OnGetDeviceData.call(self, { rgdod, rgdod + (*pdwInOut) });
    }

    if (DInput8Hook::disableInGameMouse)
    {
        *pdwInOut = 0;
    }

    return result;
}

bool DInput8Hook::Install()
{
    Core::Logger->info("Initializing DInput8 hooks...");

    HMODULE libDInput = ::GetModuleHandle(HRY_TEXT("dinput8.dll"));

    if (libDInput == nullptr)
    {
        Core::Logger->error("Cannot find dinput8.dll");
        return false;
    }

    auto dInput8Create = (DirectInput8Create_t*)::GetProcAddress(libDInput, "DirectInput8Create");
    if (dInput8Create == nullptr)
    {
        Core::Logger->error("Cannot find DirectInput8Create inside dinput8.dll");
        return false;
    }

    IDirectInput8* DI = nullptr;
    IDirectInputDevice8* DIMouse = nullptr;

    HRESULT hr;

    hr = dInput8Create(
        ::GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DI, nullptr);

    if (FAILED(hr))
    {
        Core::Logger->error("Cannot create DInput instance");
        return false;
    }

    DIVTable = DI->lpVtbl;
    hr = IDirectInput8_CreateDevice(DI, GUID_SysMouse, &DIMouse, nullptr);

    if (FAILED(hr))
    {
        Core::Logger->error("Cannot create DInput device");
        IDirectInput8_Release(DI);
        return false;
    }

    DIDeviceVTable = DIMouse->lpVtbl;

    Core::Logger->info("Hooking DirectInputDevice::GetDeviceData...");
    oDirectInputGetDeviceData =
        HookVTableField(&DIDeviceVTable->GetDeviceData, &new_DirectInputDevice_GetDeviceData);

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
            Core::Logger->info("Restoring DirectInputDevice::GetDeviceData...");
            HookVTableField(&DIDeviceVTable->GetDeviceData, oDirectInputGetDeviceData);
        }
    }
}

HRY_NS_END
