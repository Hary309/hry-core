#pragma once

#include <Windows.h>

#include "Events/EventBridgeBase.hpp"

#include "Hry/Math/Vec2.hpp"

namespace hry::events
{

class WndProcEventBridge : public EventBridgeBase
{
private:
    math::Vec2i _lastMousePos = { 0 };

public:
    WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(const HWND, UINT, WPARAM, LPARAM);
};

}
