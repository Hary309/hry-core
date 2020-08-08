#pragma once

#include <cstdint>
#include <vector>

#include "Hry/Utils/Delegate.hpp"

struct DIDEVICEOBJECTDATA;

HRY_NS_BEGIN

struct DInput8Hook
{
    using OnMouseData_t = Delegate<void(const std::vector<DIDEVICEOBJECTDATA>&&)>;
    using OnControllerData_t = Delegate<void(
        const std::vector<DIDEVICEOBJECTDATA>&&,
        uint32_t)>; // TODO: use enum or some struct instead of uint32_t

    inline static OnMouseData_t OnMouseData;
    inline static OnControllerData_t OnControllerData;

    inline static bool DisableInGameMouse;

    static bool Install();
    static void Uninstall();
};

HRY_NS_END
