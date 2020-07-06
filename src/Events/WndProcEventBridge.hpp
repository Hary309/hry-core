#pragma once

#include <Windows.h>

#include "EventBridgeBase.hpp"

namespace hry::events
{

class WndProcEventBridge : public EventBridgeBase
{
public:
    WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(const HWND, UINT, WPARAM, LPARAM);
};

}
