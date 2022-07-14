/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


namespace hry
{
/**
 * @brief Game's window resize types
 */
enum ResizeType
{
    Maximized = 0,
    Minimized,
    Restored
};

/**
 * @brief Possible button state
 */
enum class ButtonState
{
    Pressed,
    Released
};
}
