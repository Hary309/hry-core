#pragma once

#include <vector>
#include <cstdint>

#include "Hry/Utils/Delegate.hpp"

struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

namespace hry
{

struct DInput8Hook
{
    inline static Delegate<void(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&)> OnGetDeviceData;

    inline static bool disableInGameMouse;

    static bool Install();
    static void Uninstall();
};

}

