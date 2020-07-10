#include "Core.hpp"
#include "imgui.h"
#include "scssdk.h"
#include "scssdk_telemetry_event.h"

#include <cstdio>

#include <scssdk_telemetry.h>

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Events/Event.hpp"

namespace hry
{

Core::Core(HINSTANCE hInst)
    : _renderer(*this)
{
    hInstance = hInst;
}

Core::~Core()
{
    Core::UninstallHooks();
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry) 
{
    _scsTelemetry = scsTelemetry;

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stdin);

	bool success = true;

    success &= scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_frame_start, Core::TelemetryFrameEnd, this) == SCS_RESULT_ok;
    _deltaTime.reset();

    if (success == false)
	{
		printf("Unable to register event callbacks");
		return false;
	}

	success &= Core::InstallHooks();

    _renderer.init();
    _eventMgr.init();

    return success;
}

void Core::update()
{
    float deltaTime = _deltaTime.asSeconds();
    _deltaTime.reset();

    hry::events::Event* event = _eventMgr.front();

    while (event != nullptr)
    {
        Core::ProcessImGuiEvents(event);

        _eventMgr.pop();
        event = _eventMgr.front();
    }
}

void Core::imguiRender() 
{
    static float f;

    ImGui::Begin("Test");
    ImGui::Text("Adsf");
    ImGui::InputFloat("float", &f);
    ImGui::Text("Adsf");
    ImGui::Text("Adsf");
    ImGui::Text("Adsf");
    ImGui::Text("Adsf");
    ImGui::Text("Adsf");
    ImGui::Text("Adsf");
    ImGui::End();
}

bool Core::InstallHooks()
{
    bool success = true;

    success &= hooks::D3D11Hook::Install();
    success &= hooks::DInput8Hook::Install();

    if (success)
    {
        printf("Hooks installed!\n");
    }
    else
    {
        printf("Cannot install hooks!\n");
    }

    return success;
}

void Core::UninstallHooks()
{
    hooks::D3D11Hook::Uninstall();
    hooks::DInput8Hook::Uninstall();
}

void Core::ProcessImGuiEvents(hry::events::Event* event) 
{
    auto& imguiIO = ImGui::GetIO();

    // add missing mouse events

    if (event->type == events::Event::Type::MouseButtonPressed ||
        event->type == events::Event::Type::MouseButtonReleased)
    {
        auto& buttonEvent = event->get<events::MouseButtonEvent>();
        imguiIO.MouseDown[static_cast<int>(buttonEvent.button)] = event->type == events::Event::Type::MouseButtonPressed;
    }

    if (event->type == events::Event::Type::MouseWheelScrolled)
    {
        auto& wheelEvent = event->get<events::MouseWheelEvent>();

        if (wheelEvent.wheel == system::Mouse::Wheel::Vertical)
        {
            imguiIO.MouseWheel = static_cast<float>(wheelEvent.delta);
        }
        else
        {
            imguiIO.MouseWheelH = static_cast<float>(wheelEvent.delta);
        }
    }
}

void Core::TelemetryFrameEnd(const scs_event_t event, const void* const, const scs_context_t self) 
{
    Core* core = reinterpret_cast<Core*>(self);

    core->update();
}

}
