/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <Windows.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventProxyBase.hpp"

namespace hry
{
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
}
