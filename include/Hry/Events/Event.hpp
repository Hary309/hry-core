/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"

#include <guiddef.h>

#include <cstdint>
#include <optional>
#include <variant>

namespace hry
{
/**
 * @brief Window resize event
 */
struct ResizeEvent
{
    /**
     * @brief Resize type
     */
    ResizeType type;

    /**
     * @brief New window size
     */
    Vec2u size;
};

/**
 * @brief Keyboard event
 */
struct KeyboardEvent
{
    /**
     * @brief Key that has been pressed/released
     */
    Keyboard::Key key;

    /**
     * @brief State of the key
     */
    ButtonState state;
};

/**
 * @brief Mouse's button event
 */
struct MouseButtonEvent
{
    /**
     * @brief Button that has been pressed/released
     */
    Mouse::Button button;

    /**
     * @brief State of the button
     */
    ButtonState state;
};

/**
 * @brief Mouse move event
 */
struct MouseMoveEvent
{
    /**
     * @brief The change of mouse position from last MouseMoveEvent invoke
     */
    Vec2i offset;
};

/**
 * @brief Mouse wheel event (also known as scroll)
 */
struct MouseWheelEvent
{
    /**
     * @brief Axis in which wheel moved
     */
    Mouse::Wheel wheel;

    /**
     * @brief Wheel offset from last MouseWheelEvent invoke
     */
    short delta;
};

/**
 * @brief Game state event
 */
struct GameStateEvent
{
    /**
     * @brief In what state game is in
     */
    enum class Type
    {
        Paused,
        Resumed
    } type;
};

/**
 * @brief Joystick analog move event
 */
struct JoystickMoveEvent
{
    /**
     * @brief GUID of device
     */
    GUID deviceGUID;

    /**
     * @brief Axis on which the joystick moved
     */
    Joystick::Axis axis;

    /**
     * @brief New position on the axis
     * range [-100 .. 100]
     */
    double value;
};

/**
 * @brief Joystick button event
 *
 */
struct JoystickButtonEvent
{
    /**
     * @brief GUID of device
     */
    GUID deviceGUID;

    /**
     * @brief Button that has been pressed/released
     */
    Joystick::Button button;

    /**
     * @brief State of the button
     */
    ButtonState state;
};

/**
 * @brief hry-core overlay state event
 */
struct OverlayStateEvent
{
    /**
     * @brief Is the overlay visible?
     */
    bool isEnabled;
};

/**
 * @brief Game frame start event
 */
struct FrameStartEvent
{
    /**
     * @brief Passed time since last FrameStartEvent
     */
    double deltaTime;

    /**
     * @brief Is game paused?
     */
    bool isGamePaused;
};

/**
 * @brief Game frame end event
 */
struct FrameEndEvent
{
    /**
     * @brief Passed time since last FrameEndEvent
     */
    double deltaTime;
};
}
