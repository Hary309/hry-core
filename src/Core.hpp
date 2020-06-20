#pragma once

#include <windows.h>

struct scs_telemetry_init_params_v100_t;

namespace hry
{

class Core
{
public:
    static inline HINSTANCE hInstance;

private:
    scs_telemetry_init_params_v100_t* _scsTelemetry = nullptr;

public:
    explicit Core(HINSTANCE hInst);
    ~Core();

    bool init(scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void installHooks();
    static void uninstallHooks();
};

}
