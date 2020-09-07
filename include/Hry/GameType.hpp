#pragma once

#include <string_view>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

enum class GameType
{
    ETS2,
    ATS,
    Unknown
};

inline std::string_view GameTypeToString(GameType gameType)
{
    switch (gameType)
    {
        case GameType::ETS2: return "Euro Truck Simulator 2";
        case GameType::ATS: return "American Truck Simulator";
        default: return "Unknown";
    }
}

HRY_NS_END
