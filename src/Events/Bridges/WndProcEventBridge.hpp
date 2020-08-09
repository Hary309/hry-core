#pragma once

#include <Windows.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventBridgeBase.hpp"

HRY_NS_BEGIN

class WndProcEventBridge : public EventBridgeBase
{
private:
    Sink<void(HWND, UINT, WPARAM, LPARAM)> _onWndProc;
    Vec2i _lastMousePos{};

public:
    explicit WndProcEventBridge(EventManager& eventMgr);

private:
    void onWndProc(HWND, UINT, WPARAM, LPARAM);
};

HRY_NS_END
