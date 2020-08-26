#include "MainWindow.hpp"

#include <functional>

#include <imgui.h>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

MainWindow::MainWindow(
    ModuleManager& moduleMgr,
    ConfigManager& configMgr,
    KeyBindsManager& keyBindsMgr,
    EventHandler& eventHandler)
    : _moduleMgr(moduleMgr), _configPage(configMgr), _keyBindsPage(keyBindsMgr, eventHandler)
{
}

void MainWindow::initKeyBinds(KeyBinds& keyBinds)
{
    auto* showMainWindowBind = keyBinds.createKeyBind("Show main window", "show_main_window");
    showMainWindowBind->setDefaultKey(Keyboard::Key::F9);
    showMainWindowBind->pressAction.connect<&MainWindow::showMainWindowKeyBind>(this);
}

void MainWindow::imguiRender()
{
    if (!_isWindowEnabled)
    {
        return;
    }

    ImGui::SetNextWindowSize({ 400.f, 300.f }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Plugin Manager"))
    {
        if (ImGui::BeginTabBar("MainWindow::TabBar"))
        {
            if (ImGui::BeginTabItem("Manager"))
            {
                renderPluginsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                _configPage.renderImGuiPage();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Key binds"))
            {
                _keyBindsPage.renderImGuiPage();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Plugins page"))
            {
                renderPluginsPageTab();
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

    const auto& modules = _moduleMgr.getModules();

    if (modules.empty())
    {
        ImGui::Text("No plugins");
        return;
    }

    ImGui::Columns(3);

    ImGui::Separator();
    ImGui::Text("Action");
    ImGui::NextColumn();
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Description");
    ImGui::NextColumn();
    ImGui::Separator();

    for (const auto& module : modules)
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
            const auto& pluginInfo = module->plugin->getPluginInfo();
            ImGui::Text("%s", pluginInfo.shortName.c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", pluginInfo.shortDesc.c_str());
            ImGui::NextColumn();
        }
        else
        {
            ImGui::Text("%s", module->dllName.c_str());
            ImGui::NextColumn();
            ImGui::Text("N/A");
            ImGui::NextColumn();
        }

        ImGui::PopID();
    }

    ImGui::Columns(1);
}

void MainWindow::renderPluginsPageTab()
{
    const auto& modules = _moduleMgr.getModules();
    int size = modules.size();

    if (size == 0)
    {
        ImGui::Text("No plugins");
        return;
    }

    if (_selectedPluginIndex >= size)
    {
        _selectedPluginIndex = 0;
    }

    const auto& selectedModule = modules[_selectedPluginIndex];

    ImGui::PushItemWidth(-1.f);
    if (ImGui::BeginCombo("##Select Plugin", selectedModule->dllName.c_str()))
    {
        for (int i = 0; i < size; i++)
        {
            const auto& module = modules[i];
            const bool isSelected = (_selectedPluginIndex == i);

            const auto flag =
                module->isLoaded ? ImGuiSelectableFlags_None : ImGuiSelectableFlags_Disabled;

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
        selectedModule->plugin->imguiPage();
    }
    else
    {
        ImGui::Text("This plugin isn't loaded!");
    }
}

void MainWindow::renderAboutTab()
{
    ImGui::Text("Credits etc");
}

void MainWindow::showMainWindowKeyBind(hry::ButtonState /*unused*/)
{
    _isWindowEnabled = !_isWindowEnabled;
    Mouse::DisableInGameMouse(_isWindowEnabled);

    EnableImGuiCursor(_isWindowEnabled);
}

HRY_NS_END
