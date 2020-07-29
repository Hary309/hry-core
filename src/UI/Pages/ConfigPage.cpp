#include "ConfigPage.hpp"

#include <imgui.h>

HRY_NS_BEGIN

ConfigPage::ConfigPage(ConfigManager& configMgr) : _configMgr(configMgr)
{
}

void ConfigPage::renderImGuiPage()
{
    // TODO: add saving button and info about dirty
    auto& configs = _configMgr.getConfigs();
    for (auto* config : configs)
    {
        if (config->isEmpty())
        {
            continue;
        }

        if (ImGui::CollapsingHeader(config->getName().c_str()))
        {
            config->imguiRender();
        }
    }
}

HRY_NS_END