#pragma once

#include <vector>
#include <cstdint>

#include "Hry/Utils/Delegate.hpp"

struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

namespace hry::hooks
{

struct DInput8Hook
{
    inline static utils::Delegate<void(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&)> OnGetDeviceData;

    static bool Install();
    static void Uninstall();
};

}

