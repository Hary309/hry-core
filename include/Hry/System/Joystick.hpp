/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


namespace hry
{
/**
 * @brief Joystick class
 */
struct Joystick
{
    /**
     * @brief Possible axis to use
     */
    enum class Axis
    {
        X, // X axis
        Y, // Y axis
        Z, // Z axis
        R, // R axis
        U, // U axis
        V  // V axis
    };

    /**
     * @brief Possible buttons to use
     */
    enum class Button
    {
        Button1,
        Button2,
        Button3,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8,
        Button9,
        Button10,
        Button11,
        Button12,
        Button13,
        Button14,
        Button15,
        Button16,
        Button17,
        Button18,
        Button19,
        Button20,
        Button21,
        Button22,
        Button23,
        Button24,
        Button25,
        Button26,
        Button27,
        Button28,
        Button29,
        Button30,
        Button31,
        Button32,
        DpadLeft,
        DpadRight,
        DpadUp,
        DpadDown
    };
};
}
