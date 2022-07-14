/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <windows.h>

#include "Hry/Utils/Delegate.hpp"

namespace hry
{
struct OpenGLHook
{
    inline static Delegate<void(HWND, UINT, WPARAM, LPARAM)> OnWndProc;

    inline static Delegate<void(HWND hWnd)> OnInit;
    inline static Delegate<void()> OnSwapBuffers;

    static bool Install();
    static void Uninstall();
};
}