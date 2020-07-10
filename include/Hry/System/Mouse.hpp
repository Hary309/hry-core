#pragma once

namespace hry::system
{

struct Mouse
{
    enum class Button
    {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2
    };

    enum class Wheel
    {
        Vertical,
        Horizontal
    };
};

}