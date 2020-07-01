#pragma once

#include <windows.h>

#include "Renderer/Renderer.hpp"

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

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);

    void update(float deltaTime);
    void imguiRender();

    void event(const Event& event);

private:
    static bool installHooks();
    static void uninstallHooks();
};

}
