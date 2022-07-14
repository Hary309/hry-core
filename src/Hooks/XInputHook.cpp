/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "XInputHook.hpp"

#include "Core.hpp"

#include "Hry/Memory/Detour.hpp"

#include <Windows.h>
#include <Xinput.h>
#include <psapi.h>

#include <array>
#include <limits>
#include <memory>
#include <string_view>

#undef max
#undef min

namespace hry
{
using XInputGetState_t = decltype(XInputGetState);

static std::unique_ptr<Detour> detour;

DWORD new_XInputGetState(DWORD index, XINPUT_STATE* state)
{
    auto status = detour->getOriginal<XInputGetState_t>()(index, state);

    XInputHook::OnJoystickData(index, status, *state);

    return status;
}

HMODULE GetXInputHandle()
{
    HMODULE xinputHandle = 0;

    HMODULE modules[1024];
    HANDLE proccess = GetCurrentProcess();
    DWORD needed = 0;

    if (EnumProcessModules(proccess, modules, sizeof(modules), &needed))
    {
        for (size_t i = 0; i < (needed / sizeof(HMODULE)); i++)
        {
            char moduleName[MAX_PATH]{};

            if (DWORD stringLen = GetModuleFileNameEx(
                    proccess, modules[i], moduleName, sizeof(moduleName) / sizeof(char)))
            {
                std::string_view moduleName_{ moduleName, stringLen };

                if (moduleName_.find("xinput") != std::string_view::npos)
                {
                    xinputHandle = modules[i];
                }
            }
        }
    }

    return xinputHandle;
}

bool XInputHook::Install()
{
    auto* xinputHandle = GetXInputHandle();

    if (xinputHandle == nullptr)
    {
        Core::Logger->warning("Cannot find XInput");
        return false;
    }

    auto* getState_addr = GetProcAddress(xinputHandle, "XInputGetState");

    if (getState_addr == nullptr)
    {
        Core::Logger->warning("Cannot find XInputGetState inside XInput");
        return false;
    }

    detour = std::make_unique<Detour>(
        reinterpret_cast<uintptr_t>(getState_addr),
        reinterpret_cast<uintptr_t>(new_XInputGetState));

    if (auto res = detour->create(); res != Detour::Status::Ok)
    {
        Core::Logger->error("Cannot hook XInputGetState!");
        return false;
    }

    if (auto res = detour->enable(); res != Detour::Status::Ok)
    {
        Core::Logger->error("Cannot enable hook for XInputGetState!");
        return false;
    }

    return true;
}

void XInputHook::Uninstall()
{
    detour.reset();
}
}
