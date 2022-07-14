/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ConfigPage.hpp"

#include <imgui.h>

namespace hry
{
ConfigPage::ConfigPage(ConfigManager& configMgr)
    : _configMgr(configMgr)
{
}

void ConfigPage::imguiRender()
{
    bool isAnyDirty = false;
    auto& configs = _configMgr.getConfigs();

    ImGui::BeginChild("ConfigPage", { 0, -30 });

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

    ImGui::Separator();

    if (ImGui::Button("Apply##ConfigPage"))
    {
        _configMgr.saveAll(ConfigManager::SaveType::SaveOnlyDirty);
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Apply the changes");
        ImGui::EndTooltip();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel##ConfigPage"))
    {
        for (auto* config : configs)
        {
            config->cancelChanges();
        }
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Cancel the changes");
        ImGui::EndTooltip();
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset to default"))
    {
        for (auto* config : configs)
        {
            config->resetToDefault();
        }
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Reset all fields to default values");
        ImGui::EndTooltip();
    }

    if (isAnyDirty)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 0).Value, "Unsaved changes!");
    }
}
}