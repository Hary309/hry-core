#pragma once

#include <vector>
#include <memory>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventBridgeBase.hpp"

struct scs_telemetry_init_params_v100_t;

namespace hry::events
{

class EventBridgeBase;

class EventManager
{
private:
    std::vector<std::unique_ptr<EventBridgeBase>> _eventBridges;

public:
    // system events
    utils::Signal<void(const ResizeEvent&&)> windowResizeSignal;
    utils::Signal<void()> windowGainFocusSignal;
    utils::Signal<void()> windowLoseFocusSignal;
    
    utils::Signal<void(const KeyboardEvent&&)> keyPressSignal;
    utils::Signal<void(const KeyboardEvent&&)> keyReleaseSignal;

    utils::Signal<void(const MouseButtonEvent&&)> mouseButtonPressSignal;
    utils::Signal<void(const MouseButtonEvent&&)> mouseButtonReleaseSignal;
    utils::Signal<void(const MouseMoveEvent&&)> mouseMoveSignal;
    utils::Signal<void(const MouseWheelEvent&&)> mouseWheelScrollSignal;

    utils::Signal<void()> imguiRenderSignal;
    
    // game events
    utils::Signal<void()> frameStartSignal;
    utils::Signal<void()> frameEndSignal;
    utils::Signal<void(const GameStateEvent&&)> stateChangeSignal;

public:
    void init(scs_telemetry_init_params_v100_t* scsTelemetry);

    [[nodiscard]] EventHandler createEventHandler();
};

}
