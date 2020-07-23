#pragma once

#include <memory>
#include <vector>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventBridgeBase.hpp"

struct scs_telemetry_init_params_v100_t;

namespace hry
{
class EventBridgeBase;

class EventManager
{
private:
    std::vector<std::unique_ptr<EventBridgeBase>> _eventBridges;

public:
    // system events
    Signal<void(const char*)> logSignal;

    Signal<void(const ResizeEvent&&)> windowResizeSignal;
    Signal<void()> windowGainFocusSignal;
    Signal<void()> windowLoseFocusSignal;

    Signal<void(const KeyboardEvent&&)> keyPressSignal;
    Signal<void(const KeyboardEvent&&)> keyReleaseSignal;

    Signal<void(const MouseButtonEvent&&)> mouseButtonPressSignal;
    Signal<void(const MouseButtonEvent&&)> mouseButtonReleaseSignal;
    Signal<void(const MouseMoveEvent&&)> mouseMoveSignal;
    Signal<void(const MouseWheelEvent&&)> mouseWheelScrollSignal;

    Signal<void()> imguiRenderSignal;

    // game events
    Signal<void()> frameStartSignal;
    Signal<void()> frameEndSignal;
    Signal<void(const GameStateEvent&&)> stateChangeSignal;

public:
    void init(scs_telemetry_init_params_v100_t* scsTelemetry);

    [[nodiscard]] EventHandler createEventHandler();
};

} // namespace hry
