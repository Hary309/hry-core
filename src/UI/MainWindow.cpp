/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "MainWindow.hpp"

#include <functional>

#include <imgui.h>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Utils/InternalImGuiUtils.hpp"

HRY_NS_BEGIN

bool TabButton(const char* text, bool active)
{
    if (active)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
    }

    if (auto result = ImGui::Button(text, { -1, 32 }); result && !active)
    {
        return result;
    }

    if (active)
    {
        ImGui::PopStyleColor(2);
    }

    return false;
}

MainWindow::MainWindow(
    ModuleManager& moduleMgr,
    ConfigManager& configMgr,
    KeyBindsManager& keyBindsMgr,
    EventManager& eventMgr,
    InternalEventDispatcher& eventDispatcher)
    : _pluginsPage(moduleMgr), _configPage(configMgr), _keyBindsPage(keyBindsMgr, eventDispatcher),
      _eventMgr(eventMgr)
{
}

void MainWindow::initKeyBinds(KeyBinds& keyBinds)
{
    keyBinds.add(KeyBindBuilder()
                     .setLabel("Show main window")
                     .setID("show_main_window")
                     .setDefaultKey(Keyboard::Key::Delete)
                     .setPressCallback(Dlg<&MainWindow::showMainWindowKeyBind>(this))
                     .build());
}

void MainWindow::imguiRender()
{
    if (!_isWindowEnabled)
    {
        return;
    }

    ImGui::SetNextWindowSize({ 600.f, 400.f }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Plugin Manager"))
    {
        ImGui::Columns(4, "tabs##plugin_manager", false);

        if (TabButton("Plugins", _currentPage == &_pluginsPage))
        {
            _currentPage = &_pluginsPage;
        }

        ImGui::NextColumn();

        if (TabButton("Settings", _currentPage == &_configPage))
        {
            _currentPage = &_configPage;
        }

        ImGui::NextColumn();

        if (TabButton("Key binds", _currentPage == &_keyBindsPage))
        {
            _currentPage = &_keyBindsPage;
        }

        ImGui::NextColumn();

        if (TabButton("About", _currentPage == &_aboutPage))
        {
            _currentPage = &_aboutPage;
        }

        ImGui::Columns(1);

        ImGui::Separator();

        _currentPage->imguiRender();
    }

    ImGui::End();
}

void MainWindow::showMainWindowKeyBind(hry::ButtonState /*unused*/)
{
    _isWindowEnabled = !_isWindowEnabled;
    Mouse::DisableInGameMouse(_isWindowEnabled);

    InternalImGuiUtils::EnableCursor(_isWindowEnabled);

    _eventMgr.system.overlayStateChangeSignal.call({ _isWindowEnabled });
}

HRY_NS_END
