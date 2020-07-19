#pragma once

namespace hry
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

    static void DisableInGameMouse(bool disable);
};

}