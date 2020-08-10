#include "OpenGLRendererImpl.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include "Hry/Utils.hpp"

#include "Hooks/OpenGLHook.hpp"
#include "Renderer/RendererBase.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

OpenGLRendererImpl::OpenGLRendererImpl(Renderer& renderer, EventManager& eventMgr)
    : RendererBase(renderer, eventMgr)
{
}

OpenGLRendererImpl::~OpenGLRendererImpl()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}

void OpenGLRendererImpl::init()
{
    _dllOpenGL3 = GetModuleHandle(HRY_TEXT("opengl32.dll"));
    _wglGetProcAddress = reinterpret_cast<wglGetProcAddress_t*>(gladLoader("wglGetProcAddress"));

    OpenGLHook::OnInit.connect<&OpenGLRendererImpl::onInit>(this);
    OpenGLHook::OnSwapBuffers.connect<&OpenGLRendererImpl::onSwapBuffers>(this);
    OpenGLHook::OnWndProc.connect<&OpenGLRendererImpl::onWndProc>(this);
}

void OpenGLRendererImpl::onInit(HWND hWnd)
{
    Core::Logger->info("Initializing OpenGL renderer...");

    if (gladLoadGLLoader(OpenGLRendererImpl::gladLoader) == 0)
    {
        Core::Logger->error("Cannot initialize glad loader!");
    }

    ImGui::CreateContext();

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
    _eventMgr.wndProcSignal.call(hWnd, uMsg, wParam, lParam);
}

void* OpenGLRendererImpl::gladLoader(const char* name)
{
    if (_wglGetProcAddress != nullptr)
    {
        auto proc = _wglGetProcAddress(name);
        if (proc != nullptr)
        {
            return reinterpret_cast<void*>(proc);
        }
    }

    return reinterpret_cast<void*>(GetProcAddress(_dllOpenGL3, name));
}

HRY_NS_END
