#pragma once

#include <cstdint>

#include "Delegate.hpp"

struct IDirectInputDevice8;
struct DIDEVICEOBJECTDATA;

namespace hry::hooks
{

struct DInput8Hook
{
    inline static Delegate<void(IDirectInputDevice8*, DIDEVICEOBJECTDATA*, uint32_t*)> OnGetDeviceData;

    static bool install();
    static void uninstall();
};

}

