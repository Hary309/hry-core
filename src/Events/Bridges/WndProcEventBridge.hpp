#pragma once

#include <Windows.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/Namespace.hpp"

#include "Events/EventBridgeBase.hpp"

HRY_NS_BEGIN

class WndProcEventBridge : public EventBridgeBase
{
private:
    Vec2i _lastMousePos{};

public:
    WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(const HWND, UINT, WPARAM, LPARAM);
};

HRY_NS_END
