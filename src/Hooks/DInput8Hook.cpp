#include "DInput8Hook.hpp"

#define COBJMACROS
#define CINTERFACE

#include <cstdio>
#include <memory>
#include <vector>

#include <dinput.h>
#include <windows.h>

#include "Hry/Memory/Detour.hpp"

#include "Core.hpp"

const GUID IID_IDirectInput8W{
    0xbf798031, 0x483a, 0x4da2, { 0xaa, 0x99, 0x5d, 0x64, 0xed, 0x36, 0x97, 0x00 }
};

const GUID GUID_SysMouseEm{
    0x6F1D2B80, 0xD5A0, 0x11CF, { 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 }
};

using DirectInput8Create_t = decltype(DirectInput8Create);
using DirectInputDevice8_GetDeviceData_t = decltype(IDirectInputDevice8WVtbl::GetDeviceData);

static std::unique_ptr<hry::Detour> detour;

HRY_NS_BEGIN

HRESULT __stdcall new_DirectInputDevice_GetDeviceData(
    IDirectInputDevice8W* self,
    DWORD cbObjectData,
    DIDEVICEOBJECTDATA* rgdod,
    DWORD* pdwInOut,
    DWORD dwFlags)
{
    auto result = detour->getOriginal<DirectInputDevice8_GetDeviceData_t>()(
        self, cbObjectData, rgdod, pdwInOut, dwFlags);

    if (self != nullptr && rgdod != nullptr && pdwInOut != nullptr)
    {
        DIDEVICEINSTANCEW instance;
        instance.dwSize = sizeof(DIDEVICEINSTANCEW);
        IDirectInputDevice8_GetDeviceInfo(self, &instance);

        const auto deviceType = GET_DIDEVICE_TYPE(instance.dwDevType);

        switch (deviceType)
        {
            case DI8DEVTYPE_MOUSE:
            {
                DInput8Hook::OnMouseData({ rgdod, rgdod + (*pdwInOut) });

                if (DInput8Hook::DisableInGameMouse)
                {
                    *pdwInOut = 0;
                }
            }
            break;

            case DI8DEVTYPE_KEYBOARD: break;

            default:
            {
                DInput8Hook::OnControllerData({ rgdod, rgdod + (*pdwInOut) }, instance.dwDevType);
            }
            break;
        }
    }

    return result;
}

bool DInput8Hook::Install()
{
    Core::Logger->info("Initializing DInput8 hooks...");

    HMODULE libDInput = ::GetModuleHandle(HRY_TEXT("dinput8.dll"));
    IDirectInput8W* DI = nullptr;
    IDirectInputDevice8W* DIMouse = nullptr;

    if (libDInput == nullptr)
    {
        Core::Logger->error("Cannot find dinput8.dll");
        return false;
    }

    auto dInput8Create =
        reinterpret_cast<DirectInput8Create_t*>(::GetProcAddress(libDInput, "DirectInput8Create"));
    if (dInput8Create == nullptr)
    {
        Core::Logger->error("Cannot find DirectInput8Create inside dinput8.dll");
        return false;
    }

    HRESULT hr = 0;

    hr = dInput8Create(
        ::GetModuleHandle(nullptr), 0x0800, IID_IDirectInput8W, reinterpret_cast<void**>(&DI),
        nullptr);

    if (FAILED(hr))
    {
        Core::Logger->error("Cannot create DInput instance");
        return false;
    }

    // use GUID_SysMouseEm because GUID_SysMouse and GUID_SysKeyboard overrides steamoverlay
    hr = IDirectInput8_CreateDevice(DI, GUID_SysMouseEm, &DIMouse, nullptr);

    if (FAILED(hr))
    {
        Core::Logger->error("Cannot create DInput device");
        return false;
    }

    Core::Logger->info("Hooking DirectInputDevice::GetDeviceData...");

    detour = std::make_unique<Detour>(
        DIMouse->lpVtbl->GetDeviceData, new_DirectInputDevice_GetDeviceData);

    if (auto err = detour->hook(); err != Detour::Status::Ok)
    {
        Core::Logger->error(
            "Cannot hook DirectInputDevice::GetDeviceData [{}]", static_cast<int>(err));
        return false;
    }

    if (auto err = detour->enable(); err != Detour::Status::Ok)
    {
        Core::Logger->error(
            "Cannot enable hook DirectInputDevice::GetDeviceData [{}]", static_cast<int>(err));
        return false;
    }

    IDirectInputDevice8_Release(DIMouse);
    IDirectInput8_Release(DI);

    return true;
}

void DInput8Hook::Uninstall()
{
    detour.reset();
}

HRY_NS_END
