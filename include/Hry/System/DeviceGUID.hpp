#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct DeviceGUID
{
    uint32_t data1{};
    uint16_t data2{};
    uint16_t data3{};
    std::array<uint8_t, 8> data4{};

    const std::string format() const
    {
        return fmt::format("{{{:x}-{:x}-{:x}-{:x}}}", data1, data2, data3, fmt::join(data4, ""));
    }
};

HRY_NS_END
