#include "MainWindow.hpp"

#include <imgui.h>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

#include "Hry/Utils/Delegate.hpp"
#include "Utils/ImGuiUtils.hpp"

namespace hry
{

MainWindow::MainWindow(
    ModuleManager& moduleMgr,
    KeyBindsManager& keyBindsMgr)
    : 
    _moduleMgr(moduleMgr),
    _keyBindsMgr(keyBindsMgr)
{
}

void MainWindow::setupKeyBinds(KeyBinds& keyBinds) 
{
    keyBinds.addBind(
        "show_main_window",
        "Show main window",
        Keyboard::Key::F9,
        {ConnectArg_v<&MainWindow::showMainWindowKeyBind>, this}
        );
}

void MainWindow::renderImGui() 
{
    if (!_isEnabled)
        return;

    ImGui::SetNextWindowSize({400.f, 300.f}, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Plugin Manager"))
    {
        if (ImGui::BeginTabBar("MainWindow::Tab"))
        {
            if (ImGui::BeginTabItem("Plugins"))
            {
                renderPluginsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                renderSettingsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Plugins settings"))
            {
                renderPluginsSettingsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Binds"))
            {
                renderBindsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("About"))
            {
                renderAboutTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

void MainWindow::renderPluginsTab() 
{
    if (ImGui::Button("Rescan"))
    {
        _moduleMgr.scan();
    }

    auto& modules = _moduleMgr.getModules();

    if (modules.empty())
    {
        ImGui::Text("No plugins");
        return;
    }

    ImGui::Columns(3);

    ImGui::Separator();
    ImGui::Text("Action"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Description"); ImGui::NextColumn();
    ImGui::Separator();

    for (auto& module : modules)
    {
        ImGui::PushID(module.get());

        if (module->isLoaded)
        {
            if (ImGui::SmallButton("Unload"))
            {
                _moduleMgr.unload(module.get());
            }
        }
        else
        {
            if (ImGui::SmallButton("Load"))
            {
                _moduleMgr.load(module.get());
            }
        }

        ImGui::NextColumn();

        if (module->isLoaded)
        {
            auto& pluginInfo = module->plugin->getPluginInfo();
            ImGui::Text("%s", pluginInfo.shortName.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", pluginInfo.shortDesc.c_str()); ImGui::NextColumn();
        }
        else
        {
            ImGui::Text("%s", module->dllName.c_str()); ImGui::NextColumn();
            ImGui::Text("N/A"); ImGui::NextColumn();
        }

        ImGui::PopID();
    }

    ImGui::Columns(1);
}

void MainWindow::renderSettingsTab() 
{
    ImGui::Text("Settings related to this plugin, also some developer options");
}

void MainWindow::renderPluginsSettingsTab() 
{
    auto& modules = _moduleMgr.getModules();
    auto size = modules.size();

    if (size == 0)
    {
        ImGui::Text("No plugins");
        return;
    }

    if (_selectedPluginIndex >= size)
    {
        _selectedPluginIndex = 0;
    }

    auto& selectedModule = modules[_selectedPluginIndex];

    ImGui::PushItemWidth(-1.f);
    if (ImGui::BeginCombo("##Select Plugin", selectedModule->dllName.c_str()))
    {
        for (size_t i = 0; i < size; i++)
        {
            auto& module = modules[i];
            const bool isSelected = (_selectedPluginIndex == i);

            const auto flag = module->isLoaded ? ImGuiSelectableFlags_None : ImGuiSelectableFlags_Disabled;

            if (ImGui::Selectable(module->dllName.c_str(), isSelected, flag))
            {
                _selectedPluginIndex = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (selectedModule->isLoaded)
    {
        selectedModule->plugin->imguiSettingsTab();
    }
    else
    {
        ImGui::Text("This plugin isn't loaded!");
    }
}

void MainWindow::renderBindsTab() 
{
    auto& keyBindsList = _keyBindsMgr.getKeyBinds();

    for (auto& keyBindsSection : keyBindsList)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        if (!keyBinds.empty())
        {
            if (ImGui::CollapsingHeader(keyBindsSection->getName().c_str()))
            {
                for (auto& keyBind : keyBinds)
                {
                    ImGui::Text("%s", keyBind.name.c_str());
                }
            }
        }
            
    }
}

void MainWindow::renderAboutTab() 
{
    ImGui::Text("Credits etc");
}

void MainWindow::showMainWindowKeyBind()
{
    _isEnabled = !_isEnabled;
    Mouse::DisableInGameMouse(_isEnabled);

    EnableImGui(_isEnabled);
}

}
