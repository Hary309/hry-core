#include "XInputHook.hpp"

#include <array>
#include <limits>
#include <memory>
#include <string_view>

#include <Windows.h>
#include <Xinput.h>
#include <psapi.h>

#include "Hry/Memory/Detour.hpp"

#include "Core.hpp"

#undef max
#undef min

HRY_NS_BEGIN

using XInputGetState_t = decltype(XInputGetState);

static std::unique_ptr<Detour> detour;
static XINPUT_STATE deviceState[4];

DWORD new_XInputGetState(DWORD index, XINPUT_STATE* state)
{
    auto result = detour->getOriginal<XInputGetState_t>()(index, state);

    if (result == ERROR_SUCCESS && state->dwPacketNumber > deviceState[index].dwPacketNumber)
    {
        auto& g = state->Gamepad;

        auto lt = static_cast<double>(g.bLeftTrigger) * 100.0 / std::numeric_limits<BYTE>::max();
        auto rt = static_cast<double>(g.bRightTrigger) * 100.0 / std::numeric_limits<BYTE>::max();

        auto lx = static_cast<double>(g.sThumbLX) * 100.0 / std::numeric_limits<SHORT>::max();
        auto ly = static_cast<double>(g.sThumbLY) * 100.0 / std::numeric_limits<SHORT>::max();

        auto rx = static_cast<double>(g.sThumbRX) * 100.0 / std::numeric_limits<SHORT>::max();
        auto ry = static_cast<double>(g.sThumbRY) * 100.0 / std::numeric_limits<SHORT>::max();

        Core::Logger->info(
            "LT: {}, RT: {}, LX: {}, LY: {}, RX: {}, RY: {} ", lt, rt, lx, ly, rx, ry);
    }

    return result;
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

HRY_NS_END
