/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <imgui.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

/**
 * @brief Holds color ready to use for ImGui
 *        Colors are base colors for hry-core interface design
 */
struct Colors final
{
    const inline static ImColor Orange{ 255, 174, 0 };
    const inline static ImColor Gray{ 100, 100, 100 };
};

HRY_NS_END
