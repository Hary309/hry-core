#pragma once

#include <cstdint>
#include <vector>

#include "Hry/Utils/Delegate.hpp"

struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct DInput8Hook
{
    using OnGetDeviceData_t =
        Delegate<void(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&)>;

    inline static OnGetDeviceData_t OnGetDeviceData;

    inline static bool disableInGameMouse;

    static bool Install();
    static void Uninstall();
};

HRY_NS_END
