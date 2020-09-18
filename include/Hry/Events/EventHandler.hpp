#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Event.hpp"

HRY_NS_BEGIN

struct EventHandler
{
    // system events
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
    } system;

    // game events
    struct
    {
        Sink<void(const FrameEvent&&)> onFrameStart;
        Sink<void(const FrameEvent&&)> onFrameEnd;
        Sink<void(const GameStateEvent&&)> onStateChange;

        // gameplay events
        struct
        {
            Sink<void(const scs::JobCancelled&&)> jobCancelledSignal;
            Sink<void(const scs::JobDelivered&&)> jobDeliveredSignal;
            Sink<void(const scs::PlayerFined&&)> playerFinedSignal;
            Sink<void(const scs::PlayerTollgatePaid&&)> playerTollgatePaidSignal;
            Sink<void(const scs::PlayerUseFerry&&)> playerUseFerrySignal;
            Sink<void(const scs::PlayerUseTrain&&)> playerUseTrainSignal;
        } gameplay;

        // configuration callbacks
        struct
        {
            Sink<void(const scs::Substances&&)> substancesSignal;
            Sink<void(const scs::Controls&&)> controlsSignal;
            Sink<void(const scs::HShifter&&)> hshifterSignal;
            Sink<void(const scs::Truck&&)> truckSignal;
            Sink<void(const scs::Trailer&&)> trailerSignal;
            Sink<void(const scs::Job&&)> jobSignal;
        } config;
    } game;
};

HRY_NS_END
