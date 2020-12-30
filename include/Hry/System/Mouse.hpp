/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

/**
 * @brief Mouse class
 */
struct Mouse
{
    /**
     * @brief Possible buttons
     */
    enum class Button
    {
        Left,
        Right,
        Middle,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8
    };

    /**
     * @brief Wheel possible axis move
     */
    enum class Wheel
    {
        Vertical,
        Horizontal
    };

    /**
     * @brief Disables in game mouse movement
     * 
     * @param disable Set true to disable in-game mouse
     */
    static void DisableInGameMouse(bool disable);
};

HRY_NS_END