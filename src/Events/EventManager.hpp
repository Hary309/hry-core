#pragma once

#include <vector>
#include <memory>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventBridgeBase.hpp"

namespace hry::events
{

class EventBridgeBase;

class EventManager
{
private:
    std::vector<std::unique_ptr<EventBridgeBase>> _eventBridges;

public:
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

public:
    EventManager() = default;
    ~EventManager() = default;

    void init();

    [[nodiscard]] EventHandler createEventHandler();
};

}
