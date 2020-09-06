#include "PluginsPage.hpp"

#include <imgui.h>

#include "Hry/Colors.hpp"
#include "Hry/Fonts.hpp"
#include "Hry/Plugin.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

bool ButtonColored(const char* txt, ImVec4 color, bool enabled = true)
{
    ImGui::PushStyleColor(ImGuiCol_Button, color);

    ImVec4 hsv;
    ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, hsv.x, hsv.y, hsv.z);

    if (enabled)
    {
        hsv.z /= 1.2f;
    }

    ImGui::ColorConvertHSVtoRGB(hsv.x, hsv.y, hsv.z, color.x, color.y, color.z);

    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);

    bool result = ImGui::Button(txt);
    ImGui::PopStyleColor(3);

    return result;
}

PluginsPage::PluginsPage(ModuleManager& moduleMgr) : _moduleMgr(moduleMgr)
{
}

void PluginsPage::imguiRender()
{
    if (_selectedPlugin == nullptr)
    {
        renderList();
    }
    else
    {
        renderDetail();
    }
}

void PluginsPage::renderList()
{
    ImGui::PushFont(Fonts::OpenSans_Bold_20);
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::Orange.Value);
    ImGui::Text("List of plugins");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::SameLine();

    ImGui::Dummy({ 0, 4 });

    ImGui::BeginChild("##plugins");

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnOffset(1, 70);

    ImGui::SetColumnOffset(2, ImGui::GetWindowContentRegionWidth() - 32);

    for (const auto& module : _moduleMgr.getModules())
    {
        bool isLoaded = module->isLoaded();
        const auto gray = ImColor(140, 140, 140).Value;
        const auto red = ImColor(200, 50, 50).Value;
        const auto green = ImColor(50, 200, 50).Value;

        ImGui::PushID(&module);

        if (ButtonColored("On", isLoaded ? gray : green, !isLoaded) && !isLoaded)
        {
            module->loadAtStart = true;
            _moduleMgr.load(module.get());
        }
        ImGui::SameLine();
        if (ButtonColored("Off", isLoaded ? red : gray, isLoaded) && isLoaded)
        {
            module->loadAtStart = false;
            _moduleMgr.unload(module.get());
        }

        // if user loaded or unloaded module
        isLoaded = module->isLoaded();

        ImGui::NextColumn();

        ImGui::Text("%s", module->dllName.c_str());

        if (isLoaded)
        {
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Text, Colors::Gray.Value);
            const auto version = module->plugin->getPluginInfo().version.toString();
            ImGui::Text("v%s", version.c_str());
            ImGui::PopStyleColor();
        }
        else if (module->loadResult != Plugin::Result::Ok)
        {
            ImGui::SameLine();
            ImGui::TextColored(
                ImColor(200, 50, 50).Value, "%s [%d]",
                [](Plugin::Result result) {
                    switch (result)
                    {
                        case Plugin::Result::ApiNotSupported: return "Not supported API version";
                        case Plugin::Result::GameNotSupported: return "Not supported game version";
                        case Plugin::Result::Error: return "Internal error";
                        default: return "Unknown";
                    }
                }(module->loadResult),
                module->loadResult);
        }

        ImGui::NextColumn();

        if (isLoaded)
        {
            if (ImGui::Button("..."))
            {
                _selectedPlugin = &module->plugin->getPluginInfo();
            }
        }

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::EndChild();
}

void PluginsPage::renderDetail()
{
    if (ImGui::ArrowButton("back##renderDetail", ImGuiDir_Left))
    {
        _selectedPlugin = nullptr;
        return;
    }

    ImGui::SameLine();

    ImGui::PushFont(Fonts::OpenSans_Bold_20);
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::Orange.Value);
    ImGui::Text("%s", _selectedPlugin->fullName.c_str());
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text("v%s", _selectedPlugin->version.toString().c_str());

    ImGui::PopFont();

    ImGui::PushStyleColor(ImGuiCol_Text, Colors::Gray.Value);
    const auto& authorInfo = _selectedPlugin->authorInfo;
    ImGui::Text("by %s <%s>", authorInfo.name.c_str(), authorInfo.email.c_str());
    ImGui::PopStyleColor();

    ImGui::Dummy({ 0, 4 });

    ImGui::TextWrapped("%s", _selectedPlugin->desc.c_str());
}

HRY_NS_END
