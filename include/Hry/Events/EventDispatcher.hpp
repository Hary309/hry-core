/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Event.hpp"

HRY_NS_BEGIN

/**
 * @brief Events possible to use in plugin
 * 
 */
struct EventDispatcher
{
    /**
     * @brief system events
     */
    struct
    {
        Sink<void(const ResizeEvent&&)> onWindowResize;
        Sink<void()> onWindowGainFocus;
        Sink<void()> onWindowLoseFocus;

        Sink<void(const KeyboardEvent&&)> onKeyPress;
        Sink<void(const KeyboardEvent&&)> onKeyRelease;

        Sink<void(const MouseButtonEvent&&)> onMouseButtonPress;
        Sink<void(const MouseButtonEvent&&)> onMouseButtonRelease;
        Sink<void(const MouseMoveEvent&&)> onMouseMove;
        Sink<void(const MouseWheelEvent&&)> onMouseWheelScroll;

        Sink<void(const JoystickMoveEvent&&)> onJoystickMove;
        Sink<void(const JoystickButtonEvent&&)> onJoystickButtonPress;
        Sink<void(const JoystickButtonEvent&&)> onJoystickButtonRelease;

        Sink<void()> onImGuiRender;
        Sink<void(const OverlayStateEvent&&)> onOverlayStateChange;
    } system;

    /**
     * @brief game events
     */
    struct
    {
        Sink<void(const FrameStartEvent&&)> onFrameStart;
        Sink<void(const FrameEndEvent&&)> onFrameEnd;
        Sink<void(const GameStateEvent&&)> onStateChange;

        /**
         * @brief gameplay events
         */
        struct
        {
            Sink<void(const std::optional<scs::JobCancelled>&&)> onJobCancelled;
            Sink<void(const std::optional<scs::JobDelivered>&&)> onJobDelivered;
            Sink<void(const std::optional<scs::PlayerFined>&&)> onPlayerFined;
            Sink<void(const std::optional<scs::PlayerTollgatePaid>&&)> onPlayerTollgatePaid;
            Sink<void(const std::optional<scs::PlayerUseFerry>&&)> onPlayerUseFerry;
            Sink<void(const std::optional<scs::PlayerUseTrain>&&)> onPlayerUseTrain;
        } gameplay;

        /**
         * @brief configuration callbacks
         * 
         * Invoke when values are changed
         */
        struct
        {
            Sink<void(const std::optional<scs::Substances>&&)> onSubstances;
            Sink<void(const std::optional<scs::Controls>&&)> onControls;
            Sink<void(const std::optional<scs::HShifter>&&)> onHShifter;
            Sink<void(const std::optional<scs::Truck>&&)> onTruck;
            Sink<void(const std::optional<scs::Trailer>&&)> onTrailer;
            Sink<void(const std::optional<scs::Job>&&)> onJob;
        } config;
    } game;
};

HRY_NS_END
