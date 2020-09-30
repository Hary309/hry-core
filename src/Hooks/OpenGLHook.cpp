/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "OpenGLHook.hpp"

#include <memory>

#include <Windows.h>
#include <gl/GL.h>

#include "Hry/Memory/Detour.hpp"
#include "Hry/Utils/Utils.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

using glViewport_t = decltype(glViewport);
using wglCreateContext_t = decltype(wglCreateContext);
using wglGetCurrentContext_t = decltype(wglGetCurrentContext);
using wglMakeCurrent_t = decltype(wglMakeCurrent);

static wglCreateContext_t* wglCreateContext_func;
static wglGetCurrentContext_t* wglGetCurrentContext_func;
static wglMakeCurrent_t* wglMakeCurrent_func;

static std::unique_ptr<Detour> wglSwapBuffer_Detour;
static std::unique_ptr<Detour> glViewPort_Detour;

static bool isInited = false;
static HWND hWnd;
static WNDPROC oWndProc;
static HGLRC hGRLC;

LRESULT __stdcall WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OpenGLHook::OnWndProc(hWnd, uMsg, wParam, lParam);

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL new_wglSwapBuffers(HDC hdc)
{
    if (!isInited)
    {
        // it works fine until you unload this library
        // crash for some reason after using wglMakeCurrent_func(hdc, hGRLC);
        // TODO: investigate for possible fix
        hGRLC = wglCreateContext_func(hdc);
        hWnd = WindowFromDC(hdc);
        Core::Logger->info("Hooking WndProc...");
        oWndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProcOpenGL));

        OpenGLHook::OnInit(hWnd);

        isInited = true;
    }

    HGLRC oHGLRC = wglGetCurrentContext_func();
    wglMakeCurrent_func(hdc, hGRLC);

    OpenGLHook::OnSwapBuffers();

    wglMakeCurrent_func(hdc, oHGLRC);
    return wglSwapBuffer_Detour->getOriginal<BOOL(HDC)>()(hdc);
}

// TODO: add support for resize
void new_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    printf("Resize: %d %d %d %d\n", x, y, width, height);

    glViewPort_Detour->getOriginal<glViewport_t>()(x, y, width, height);
}

bool OpenGLHook::Install()
{
    HMODULE libOpenGL = ::GetModuleHandle(HRY_TEXT("opengl32.dll"));
    if (libOpenGL == nullptr)
    {
        return false;
    }

    auto wglSwapBuffers_addr = GetProcAddress(libOpenGL, "wglSwapBuffers");
    wglCreateContext_func =
        reinterpret_cast<wglCreateContext_t*>(GetProcAddress(libOpenGL, "wglCreateContext"));
    wglGetCurrentContext_func = reinterpret_cast<wglGetCurrentContext_t*>(
        GetProcAddress(libOpenGL, "wglGetCurrentContext"));
    wglMakeCurrent_func =
        reinterpret_cast<wglMakeCurrent_t*>(GetProcAddress(libOpenGL, "wglMakeCurrent"));

    bool foundFuncs = true;
    foundFuncs &= wglSwapBuffers_addr != nullptr;
    foundFuncs &= wglCreateContext_func != nullptr;
    foundFuncs &= wglGetCurrentContext_func != nullptr;
    foundFuncs &= wglMakeCurrent_func != nullptr;

    if (!foundFuncs)
    {
        Core::Logger->error("Cannot find functions inside opengl32.dll");
        return false;
    }

    Core::Logger->info("Using OpenGL renderer");
    Core::Logger->info("Hooking wglSwapBuffers...");

    wglSwapBuffer_Detour = std::make_unique<Detour>(
        reinterpret_cast<uintptr_t>(wglSwapBuffers_addr),
        reinterpret_cast<uintptr_t>(new_wglSwapBuffers));

    if (auto res = wglSwapBuffer_Detour->create(); res != Detour::Status::Ok)
    {
        Core::Logger->error("Cannot hook wglSwapBuffers!");
        return false;
    }

    if (auto res = wglSwapBuffer_Detour->enable(); res != Detour::Status::Ok)
    {
        Core::Logger->error("Cannot enable hook for wglSwapBuffers!");
        return false;
    }

    // auto glViewport_addr = reinterpret_cast<glViewport_t*>(GetProcAddress(libOpenGL, "glViewport"));

    // Core::Logger->info("Hooking glViewport...");

    // glViewPort_Detour = std::make_unique<Detour>(glViewport_addr, new_glViewport);

    // if (auto res = glViewPort_Detour->create(); res != Detour::Status::Ok)
    // {
    //     Core::Logger->error("Cannot hook glViewport!");
    //     return false;
    // }

    // if (auto res = glViewPort_Detour->enable(); res != Detour::Status::Ok)
    // {
    //     Core::Logger->error("Cannot enable hook for glViewport!");
    //     return false;
    // }

    return true;
}

void OpenGLHook::Uninstall()
{
    wglSwapBuffer_Detour.reset();
    glViewPort_Detour.reset();

    if (oWndProc != nullptr)
    {
        Core::Logger->info("Restoring WndProc...");
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    }
}

HRY_NS_END
