#include "KeyBindsManager.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <ios>

#include <nlohmann/json.hpp>

#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

HRY_NS_BEGIN

KeyBindsManager::KeyBindsManager(EventHandler& eventHandler)
{
    eventHandler.onKeyPress.connect<&KeyBindsManager::handleKeybaordEvent>(this);
    eventHandler.onKeyRelease.connect<&KeyBindsManager::handleKeybaordEvent>(this);
    eventHandler.onMouseButtonPress.connect<&KeyBindsManager::handleMouseButtonEvent>(this);
    eventHandler.onMouseButtonRelease.connect<&KeyBindsManager::handleMouseButtonEvent>(this);
}

DelegateDeleterUniquePtr_t<KeyBinds> KeyBindsManager::createKeyBinds(const std::string& name)
{
    auto* keyBinds = new KeyBinds(name);
    _keyBinds.push_back(keyBinds);

    // use custom deleter to remove from list when KeyBinds is removing
    return { keyBinds, { ConnectArg_v<&KeyBindsManager::keyBindsDeleter>, this } };
}

void KeyBindsManager::remove(const KeyBinds* keyBinds)
{
    _keyBinds.erase(std::remove(_keyBinds.begin(), _keyBinds.end(), keyBinds));
}

void KeyBindsManager::keyBindsDeleter(KeyBinds* ptr)
{
    remove(ptr);
    delete ptr;
}

void KeyBindsManager::handleKeybaordEvent(const KeyboardEvent&& keyboardEvent)
{
    processKey(keyboardEvent.key, keyboardEvent.state);
}

void KeyBindsManager::handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent)
{
    processKey(buttonEvent.button, buttonEvent.state);
}

void KeyBindsManager::processKey(BindableKey::Key_t key, ButtonState buttonState)
{
    for (auto& keyBindsSection : _keyBinds)
    {
        auto& keyBinds = keyBindsSection->getKeyBinds();

        for (auto& keyBind : keyBinds)
        {
            if (keyBind.getKey() != nullptr && keyBind.getKeyState() != buttonState &&
                keyBind.getKey()->key == key)
            {
                switch (buttonState)
                {
                    case ButtonState::Pressed: keyBind.pressAction.call(); break;
                    case ButtonState::Released: keyBind.releaseAction.call(); break;
                }
                keyBind.setKeyState(buttonState);
            }
        }
    }
}

void KeyBindsManager::save()
{
    nlohmann::json json;

    std::ifstream fileOpen(FilePath);

    // don't delete keybinds for modules that aren't loaded
    if (fileOpen.good())
    {
        fileOpen >> json;
    }

    Core::Logger->info("Saving keybinds...");

    for (auto* keyBindsSection : _keyBinds)
    {
        auto jKeyBindsSection = nlohmann::json::object();

        keyBindsSection->toJson(jKeyBindsSection);

        json[keyBindsSection->getName()] = jKeyBindsSection;
    }

    if (!fs::exists(ConfigDirectory))
    {
        fs::create_directory(ConfigDirectory);
    }

    std::ofstream fileSave(FilePath);

    if (fileSave.is_open())
    {
        fileSave << json.dump(4);
    }
    else
    {
        Core::Logger->error("Cannot save keybinds to ", FilePath);
    }
}

void KeyBindsManager::loadFor(KeyBinds* keyBinds)
{
    std::ifstream file(FilePath);

    if (file.good())
    {
        Core::Logger->info("Loading keybinds for ", keyBinds->getName(), "...");
        nlohmann::json json;
        file >> json;

        auto jKeyBindsSection = json.find(keyBinds->getName());

        if (jKeyBindsSection != json.end())
        {
            keyBinds->fromJson(jKeyBindsSection.value());
        }
    }
    else
    {
        Core::Logger->warning("Cannot open ", FilePath);
    }
}

HRY_NS_END
