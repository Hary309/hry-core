/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string_view>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

/**
 * @brief Game name
 */
enum class GameType
{
    ETS2,
    ATS,
    Unknown
};

/**
 * @brief Converts game type to string
 * 
 * @param gameType Value for game type
 * @return Game name in string format
 */
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
