/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <Windows.h>

#include "Hry/Namespace.hpp"

#include "Renderer/RendererBase.hpp"

HRY_NS_BEGIN

class Renderer;

class OpenGLRendererImpl : public RendererBase
{
private:
    using wglGetProcAddress_t = decltype(wglGetProcAddress);

private:
    inline static HMODULE _dllOpenGL3;

public:
    OpenGLRendererImpl(Renderer& renderer, EventManager& eventMgr);
    ~OpenGLRendererImpl() override;

    void init() override;

private:
    void onInit(HWND hWnd);
    void onSwapBuffers();
    void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

HRY_NS_END