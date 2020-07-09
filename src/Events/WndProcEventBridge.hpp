#pragma once

#include <Windows.h>

#include "EventBridgeBase.hpp"

namespace hry::events
{

class WndProcEventBridge : public EventBridgeBase
{
private:
    int _lastMousePosX = 0;
    int _lastMousePosY = 0;

public:
    WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(const HWND, UINT, WPARAM, LPARAM);
};

}
