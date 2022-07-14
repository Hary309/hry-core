/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "OpenGLRendererImpl.hpp"

#include "Core.hpp"

#include "Hooks/OpenGLHook.hpp"
#include "Renderer/RendererBase.hpp"

#include "Hry/Utils/Utils.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

namespace hry
{
OpenGLRendererImpl::OpenGLRendererImpl(Renderer& renderer, EventManager& eventMgr)
    : RendererBase(renderer, eventMgr)
{
}

OpenGLRendererImpl::~OpenGLRendererImpl()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

void OpenGLRendererImpl::init()
{
    _dllOpenGL3 = GetModuleHandle(HRY_TEXT("opengl32.dll"));

    OpenGLHook::OnInit.connect<&OpenGLRendererImpl::onInit>(this);
    OpenGLHook::OnSwapBuffers.connect<&OpenGLRendererImpl::onSwapBuffers>(this);
    OpenGLHook::OnWndProc.connect<&OpenGLRendererImpl::onWndProc>(this);
}

void OpenGLRendererImpl::onInit(HWND hWnd)
{
    Core::Logger->info("Initializing OpenGL renderer...");

    if (gladLoadGL() == 0)
    {
        Core::Logger->error("Cannot initialize glad loader!");
    }

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplOpenGL3_Init();

    _renderer.onRendererInit();
}

void OpenGLRendererImpl::onSwapBuffers()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    _renderer.onRendererRenderImGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLRendererImpl::onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    _eventMgr.system.wndProcSignal.call(hWnd, uMsg, wParam, lParam);
}
}
