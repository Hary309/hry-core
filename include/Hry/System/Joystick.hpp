#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct Joystick
{
    enum class Axis
    {
        X,    // X axis
        Y,    // Y axis
        Z,    // Z axis
        R,    // R axis
        U,    // U axis
        V,    // V axis
        PovX, // X axis of the point-of-view hat
        PovY  // Y axis of the point-of-view hat
    };

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
        Button32
    };
};

HRY_NS_END
