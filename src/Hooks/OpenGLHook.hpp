#pragma once

#include <windows.h>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

struct OpenGLHook
{
    inline static Delegate<void(HWND, UINT, WPARAM, LPARAM)> OnWndProc;

    inline static Delegate<void(HWND hWnd)> OnInit;
    inline static Delegate<void()> OnSwapBuffers;

    static bool Install();
    static void Uninstall();
};

HRY_NS_END