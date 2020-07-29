#pragma once

#include "Hry/Namespace.hpp"

#include "Config/ConfigManager.hpp"

HRY_NS_BEGIN

class ConfigPage
{
private:
    ConfigManager& _configMgr;

public:
    explicit ConfigPage(ConfigManager& configMgr);

    void renderImGuiPage();
};

HRY_NS_END
