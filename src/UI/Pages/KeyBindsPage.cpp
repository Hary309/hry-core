#include "KeyBindsPage.hpp"

#include <imgui.h>

#include "Hry/KeyBinding/KeyBinds.hpp"

#include "Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

KeyBindsPage::KeyBindsPage(KeyBindsManager& keyBindsMgr, EventHandler& eventHandler)
    : _keyBindsMgr(keyBindsMgr)
{
    eventHandler.onKeyPress.connect<&KeyBindsPage::handleKeyPress>(this);
    eventHandler.onMouseButtonPress.connect<&KeyBindsPage::handleMouseButtonPress>(this);
    eventHandler.onJoystickButtonPress.connect<&KeyBindsPage::handleJoystickButtonPress>(this);
}

void KeyBindsPage::renderImGuiPage()
{
    const auto& keyBindsList = _keyBindsMgr.getKeyBinds();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4, 8 });

    for (const auto& keyBindsSection : keyBindsList)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        if (keyBinds.empty())
        {
            continue;
        }

        if (ImGui::CollapsingHeader(keyBindsSection->getName().c_str()))
        {
            ImGui::Columns(3);
            for (auto& keyBind : keyBinds)
            {
                ImGui::PushID(&keyBind);

                ImGui::Text("%s", keyBind->getName().c_str());

                ImGui::NextColumn();

                const auto* key = keyBind->getKey();

                if (_keyToSetBind == keyBind.get())
                {
                    ImGui::Text("Press key to bind");
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

                    if (ImGui::SmallButton(text.data()))
                    {
                        _keyToSetBind = keyBind.get();
                        EnableImGuiCursor(false);
                    }
                }

                ImGui::NextColumn();

                bool checkBoxValue = keyBind->_activator == KeyBind::Activator::Hold;

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 0 });

                if (ImGui::Checkbox("Hold", &checkBoxValue))
                {
                    keyBind->_activator =
                        checkBoxValue ? KeyBind::Activator::Hold : KeyBind::Activator::Click;
                }

                ImGui::PopStyleVar();

                ImGui::SameLine();

                if (ImGui::SmallButton("Default##KeyBinds"))
                {
                    keyBind->setKey(keyBind->getDefaultKey());
                    keyBind->_activator = keyBind->_defaultActivator;
                    _keyBindsMgr.save();
                }

                ImGui::SameLine();

                if (ImGui::SmallButton("Delete##KeyBinds"))
                {
                    keyBind->setKey(nullptr);
                    _keyBindsMgr.save();
                }

                ImGui::PopID();
                ImGui::NextColumn();
            }

            ImGui::Columns(1);
        }
    }

    ImGui::PopStyleVar();
}

void KeyBindsPage::handleKeyPress(const KeyboardEvent&& keyboardEvent)
{
    if (_keyToSetBind != nullptr)
    {
        if (keyboardEvent.key != Keyboard::Key::Escape)
        {
            _keyToSetBind->setKey(keyboardEvent.key);
            _keyToSetBind->_joystickGUID.reset();
        }

        applyChanges();
    }
}

void KeyBindsPage::handleMouseButtonPress(const MouseButtonEvent&& buttonEvent)
{
    if (_keyToSetBind != nullptr)
    {
        _keyToSetBind->setKey(buttonEvent.button);
        _keyToSetBind->_joystickGUID.reset();

        applyChanges();
    }
}

void KeyBindsPage::handleJoystickButtonPress(const JoystickButtonEvent&& buttonEvent)
{
    if (_keyToSetBind != nullptr)
    {
        _keyToSetBind->setKey(buttonEvent.button);
        _keyToSetBind->_joystickGUID = buttonEvent.deviceGUID;

        applyChanges();
    }
}

void KeyBindsPage::applyChanges()
{
    _keyToSetBind = nullptr;
    EnableImGuiCursor(true);
    _keyBindsMgr.save();
}

HRY_NS_END
