#include "Core.hpp"

#include <scssdk_telemetry.h>

namespace hry
{

Core::Core(HINSTANCE hInst)
{
    hInstance = hInst;
}

Core::~Core()
{
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry) 
{
    _scsTelemetry = scsTelemetry;

    return true;
}

}
