#pragma once

#include <Windows.h>

#include "Events/EventBridgeBase.hpp"

#include "Hry/Math/Vec2.hpp"

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

}
