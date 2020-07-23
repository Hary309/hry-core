#pragma once

#include <Windows.h>

#include "Hry/Math/Vec2.hpp"

#include "Events/EventBridgeBase.hpp"

namespace hry
{
class WndProcEventBridge : public EventBridgeBase
{
private:
    Vec2i _lastMousePos = { 0 };

public:
    WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(const HWND, UINT, WPARAM, LPARAM);
};

} // namespace hry
