/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "KeyBindsPage.hpp"

#include <imgui.h>

#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

#include "Utils/InternalImGuiUtils.hpp"

HRY_NS_BEGIN

KeyBindsPage::KeyBindsPage(KeyBindsManager& keyBindsMgr, InternalEventDispatcher& dispatcher)
    : _keyBindsMgr(keyBindsMgr)
{
    dispatcher.system.onKeyPress.connect<&KeyBindsPage::handleKeyPress>(this);
    dispatcher.system.onMouseButtonPress.connect<&KeyBindsPage::handleMouseButtonPress>(this);
    dispatcher.system.onJoystickButtonPress.connect<&KeyBindsPage::handleJoystickButtonPress>(this);
}

void KeyBindsPage::imguiRender()
{
    const auto& keyBindsList = _keyBindsMgr.getKeyBinds();

    for (const auto& keyBindsSection : keyBindsList)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        if (keyBinds.empty())
        {
            continue;
        }

        if (ImGui::CollapsingHeader(
                keyBindsSection->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Columns(3);
            ImGui::SetColumnOffset(2, ImGui::GetWindowContentRegionWidth() - 96);
            for (auto& keyBind : keyBinds)
            {
                ImGui::PushID(&keyBind);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

                ImGui::Text("%s", keyBind->label.c_str());

                if (!keyBind->desc.empty())
                {
                    ImGui::SameLine();
                    ImGuiHelpMarker(keyBind->desc.c_str());
                }

                ImGui::NextColumn();

                const auto* key = keyBind->key;

                if (_keyToSetBind == keyBind.get())
                {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                    ImGui::Text("Press key to bind");
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
                }
                else
                {
                    std::string_view text{};

                    if (key != nullptr)
                    {
                        text = key->name;
                    }
                    else
                    {
                        text = "Not set";
                    }

                    if (ImGui::Button(text.data()))
                    {
                        _keyToSetBind = keyBind.get();
                        InternalImGuiUtils::EnableCursor(false);
                    }
                }

                ImGui::NextColumn();

                if (ImGui::Button("Default##KeyBinds"))
                {
                    keyBind->key = keyBind->defaultKey;
                    keyBindsSection->saveToFile();
                }

                ImGui::SameLine();

                if (ImGui::Button("Unset##KeyBinds"))
                {
                    keyBind->key = nullptr;
                    keyBindsSection->saveToFile();
                }

                ImGui::PopID();
                ImGui::NextColumn();
            }

            ImGui::Columns(1);
        }
    }
}

void KeyBindsPage::handleKeyPress(const KeyboardEvent&& keyboardEvent)
{
    if (_keyToSetBind != nullptr)
    {
        if (keyboardEvent.key != Keyboard::Key::Escape)
        {
            _keyToSetBind->key = GetBindableKey(keyboardEvent.key);
            _keyToSetBind->joystickGUID.reset();
        }

        applyChanges();
    }
}

void KeyBindsPage::handleMouseButtonPress(const MouseButtonEvent&& buttonEvent)
{
    if (_keyToSetBind != nullptr)
    {
        _keyToSetBind->key = GetBindableKey(buttonEvent.button);
        _keyToSetBind->joystickGUID.reset();

        applyChanges();
    }
}

void KeyBindsPage::handleJoystickButtonPress(const JoystickButtonEvent&& buttonEvent)
{
    if (_keyToSetBind != nullptr)
    {
        _keyToSetBind->key = GetBindableKey(buttonEvent.button);
        _keyToSetBind->joystickGUID = buttonEvent.deviceGUID;

        applyChanges();
    }
}

void KeyBindsPage::applyChanges()
{
    _keyToSetBind = nullptr;
    InternalImGuiUtils::EnableCursor(true);
    _keyBindsMgr.saveAll();
}

HRY_NS_END
