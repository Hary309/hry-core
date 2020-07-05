#pragma once

#include <windows.h>

#include "Renderer/Renderer.hpp"
#include "Events/EventManager.hpp"
#include "Utils/Timer.hpp"

#include <scssdk_telemetry.h>

struct scs_telemetry_init_params_v100_t;

namespace hry
{

class Event;

class Core
{
public:
    static inline HINSTANCE hInstance;

private:
    scs_telemetry_init_params_v100_t* _scsTelemetry = nullptr;

    renderer::Renderer _renderer;
    events::EventManager _eventMgr;

    utils::Timer _deltaTime;

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);

    void update();
    void imguiRender();

    void event(const Event& event);

private:
    static bool installHooks();
    static void uninstallHooks();

    static void TelemetryFrameEnd(const scs_event_t event, const void* const, const scs_context_t self);
};

}
