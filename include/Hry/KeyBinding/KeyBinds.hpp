/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <guiddef.h>
#include <nlohmann/json_fwd.hpp>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/TaskScheduler.hpp"

#include "BindableKeys.hpp"

HRY_NS_BEGIN

class KeyBinds;
class KeyBindsManager;
class KeyBindsPage;
class KeyBindBuilder;

class KeyBind final
{
    friend KeyBindBuilder;

public:
    using Delegate_t = Delegate<void(ButtonState)>;

    enum class Activator
    {
        Click = 0,
        Hold
    };

public:
    std::string id;
    std::string label;
    std::string desc;

    const BindableKey* defaultKey = nullptr; // if null, not set
    const BindableKey* key = nullptr;        // if null, not set

    Activator activator = Activator::Click;

    std::optional<GUID> joystickGUID;

    // time point when key was pressed
    std::chrono::system_clock::time_point keyPressTimePoint;

    ButtonState state = ButtonState::Released;

    Delegate_t pressAction;
    Delegate_t releaseAction;

private:
    KeyBind() = default;
};

class KeyBinds
{
    friend KeyBindsManager;
    friend KeyBindsPage;

private:
    std::string _name;
    std::string _keyBindsFilePath;

    std::vector<std::unique_ptr<KeyBind>> _keyBinds;

public:
    explicit KeyBinds(std::string name);

    HRY_API KeyBind* add(std::unique_ptr<KeyBind>&& keyBind);

    void saveToFile() const;
    // return false if cannot save
    bool loadFromFile();

private:
    const std::string& getName() const { return _name; }

    auto& getKeyBinds() { return _keyBinds; }
    const auto& getKeyBinds() const { return _keyBinds; }

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);
};

class KeyBindBuilder final
{
    friend KeyBinds;

private:
    std::string _id;
    std::string _label;
    std::string _desc;

    BindableKey::Key_t _defaultKey{};

    KeyBind::Activator _activator = KeyBind::Activator::Click;

    KeyBind::Delegate_t _pressAction;
    KeyBind::Delegate_t _releaseAction;

public:
    KeyBindBuilder() = default;

    // set identifier of keybind (this will be saved to file)
    KeyBindBuilder& setID(const std::string& id)
    {
        _id = id;
        return *this;
    }

    // set display label
    KeyBindBuilder& setLabel(const std::string& label)
    {
        _label = label;
        return *this;
    }

    // set descrption, will be shown next to label
    KeyBindBuilder& setDescription(const std::string& desc)
    {
        _desc = desc;
        return *this;
    }

    KeyBindBuilder& setDefaultKey(BindableKey::Key_t key)
    {
        _defaultKey = key;
        return *this;
    }

    // set how this keybind can be triggered, by default is Activator::Click
    KeyBindBuilder& setActivator(KeyBind::Activator activator)
    {
        _activator = activator;
        return *this;
    }

    KeyBindBuilder& setPressCallback(KeyBind::Delegate_t callback)
    {
        _pressAction = callback;
        return *this;
    }

    KeyBindBuilder& setReleaseCallback(KeyBind::Delegate_t callback)
    {
        _releaseAction = callback;
        return *this;
    }

    std::unique_ptr<KeyBind> build()
    {
        auto* keyBind = new KeyBind();

        const auto* bindableKey = GetBindableKey(_defaultKey);

        keyBind->id = _id;
        keyBind->label = _label;
        keyBind->desc = _desc;
        keyBind->defaultKey = bindableKey;
        keyBind->key = bindableKey;
        keyBind->activator = _activator;
        keyBind->pressAction = _pressAction;
        keyBind->releaseAction = _releaseAction;

        return std::unique_ptr<KeyBind>(keyBind);
    }
};

HRY_NS_END
