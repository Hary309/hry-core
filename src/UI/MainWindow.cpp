#include "MainWindow.hpp"

#include <functional>

#include <imgui.h>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Utils/ImGuiUtils.hpp"

namespace hry
{
MainWindow::MainWindow(
    ModuleManager& moduleMgr, KeyBindsManager& keyBindsMgr, EventManager& eventMgr)
    : _moduleMgr(moduleMgr), _keyBindsMgr(keyBindsMgr), _onKeyPress(eventMgr.keyPressSignal),
      _onMouseButtonPress(eventMgr.mouseButtonPressSignal)
{
    _onKeyPress.connect<&MainWindow::handleKeyPress>(this);
    _onMouseButtonPress.connect<&MainWindow::handleMouseButtonPress>(this);
}

void MainWindow::initKeyBinds(KeyBinds& keyBinds)
{
    KeyBind showMainWindowBind;
    showMainWindowBind.setConfigFieldName("show_main_window");
    showMainWindowBind.setName("Show main window");
    showMainWindowBind.setDefaultKey(Keyboard::Key::F9);
    showMainWindowBind.pressAction.connect<&MainWindow::showMainWindowKeyBind>(this);
    keyBinds.addKeyBind(std::move(showMainWindowBind));
}

void MainWindow::renderImGui()
{
    if (!_isWindowEnabled)
        return;

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
                renderSettingsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Plugins page"))
            {
                renderPluginsPageTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Key binds"))
            {
                renderKeyBindsTab();
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
    ImGui::Text("Action");
    ImGui::NextColumn();
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Description");
    ImGui::NextColumn();
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

void MainWindow::renderSettingsTab()
{
    ImGui::Text("Settings related to this plugin, also some developer options");
}

void MainWindow::renderPluginsPageTab()
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

void MainWindow::renderKeyBindsTab()
{
    auto& keyBindsList = _keyBindsMgr.getKeyBinds();

    for (auto& keyBindsSection : keyBindsList)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        if (!keyBinds.empty())
        {
            if (ImGui::CollapsingHeader(keyBindsSection->getName().c_str()))
            {
                ImGui::Columns(2);

                for (auto& keyBind : keyBinds)
                {
                    ImGui::PushID(&keyBind);

                    ImGui::Text("%s", keyBind.getName().c_str());

                    ImGui::NextColumn();

                    auto key = keyBind.getKey();

                    if (_keyToSetBind == &keyBind)
                    {
                        ImGui::Text("Press key to bind");
                    }
                    else
                    {
                        const char* text;

                        if (key)
                            text = key->name.c_str();
                        else
                            text = "Not set";

                        if (ImGui::SmallButton(text))
                        {
                            _keyToSetBind = &keyBind;
                            EnableImGui(false);
                        }
                    }

                    ImGui::PopID();
                    ImGui::NextColumn();
                }

                ImGui::Columns(1);
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
    _isWindowEnabled = !_isWindowEnabled;
    Mouse::DisableInGameMouse(_isWindowEnabled);

    EnableImGui(_isWindowEnabled);
}

void MainWindow::handleKeyPress(const KeyboardEvent&& keyboardEvent)
{
    if (_keyToSetBind)
    {
        if (keyboardEvent.key != Keyboard::Key::Escape)
        {
            _keyToSetBind->setKey(keyboardEvent.key);
        }

        EnableImGui(true);
        _keyToSetBind = nullptr;
    }
}

void MainWindow::handleMouseButtonPress(const MouseButtonEvent&& buttonEvent)
{
    if (_keyToSetBind)
    {
        _keyToSetBind->setKey(buttonEvent.button);
        _keyToSetBind = nullptr;
        EnableImGui(true);
    }
}

} // namespace hry
