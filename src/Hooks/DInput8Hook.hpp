#pragma once

#include <vector>
#include <cstdint>

#include "Delegate.hpp"

struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

namespace hry::hooks
{

struct DInput8Hook
{
    inline static Delegate<void(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&)> OnGetDeviceData;

    static bool Install();
    static void Uninstall();
};

}

