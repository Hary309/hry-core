#include "ConfigPage.hpp"

#include <imgui.h>

HRY_NS_BEGIN

ConfigPage::ConfigPage(ConfigManager& configMgr) : _configMgr(configMgr)
{
}

void ConfigPage::imguiRender()
{
    bool isAnyDirty = false;
    auto& configs = _configMgr.getConfigs();

    ImGui::BeginChild("ConfigPage", { 0, -24 });

    for (auto* config : configs)
    {
        const auto& fields = config->getFields();

        if (fields.empty())
        {
            continue;
        }

        std::string headerName = config->getName();

        if (bool isDirty = config->isDirty())
        {
            isAnyDirty |= isDirty;
            headerName += '*';
        }

        ImGui::PushID(config);

        if (ImGui::CollapsingHeader(config->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            config->imguiRender();
        }

        ImGui::PopID();
    }

    ImGui::EndChild();

    if (ImGui::Button("Save##ConfigPage"))
    {
        for (auto* config : configs)
        {
            config->applyChanges();
        }

        _configMgr.saveAll();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel##ConfigPage"))
    {
        for (auto* config : configs)
        {
            config->cancelChanges();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset to default"))
    {
        for (auto* config : configs)
        {
            config->resetToDefault();
        }
    }

    if (isAnyDirty)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 0).Value, "Unsaved changes!");
    }
}

HRY_NS_END