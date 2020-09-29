#pragma once

#include <Windows.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventProxyBase.hpp"

HRY_NS_BEGIN

class WndProcEventProxy : public EventProxyBase
{
private:
    Sink<void(HWND, UINT, WPARAM, LPARAM)> _onWndProc;
    Vec2i _lastMousePos{};

public:
    explicit WndProcEventProxy(EventManager& eventMgr);

private:
    void onWndProc(HWND, UINT, WPARAM, LPARAM) noexcept;
};

HRY_NS_END
