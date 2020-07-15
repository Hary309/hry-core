#include "EventManager.hpp"

#include "Core.hpp"

#include "WndProcEventBridge.hpp"
#include "DInput8EventBridge.hpp"

namespace hry::events
{


void EventManager::init() 
{
    Core::Logger->info("Initializing EventManager...");

    _eventBridges.push_back(std::make_unique<WndProcEventBridge>(*this));
    _eventBridges.push_back(std::make_unique<DInput8EventBridge>(*this));
}

EventHandler EventManager::createEventHandler() 
{
    return {
        windowResizeSignal,
        windowGainFocusSignal,
        windowLoseFocusSignal,

        keyPressSignal,
        keyReleaseSignal,

        mouseButtonPressSignal,
        mouseButtonReleaseSignal,
        mouseMoveSignal,
        mouseWheelScrollSignal,

        imguiRenderSignal,
    };
}

}