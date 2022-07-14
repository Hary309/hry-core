/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "BindableKeys.hpp"

#include "Hry/Export.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/TaskScheduler.hpp"

#include <guiddef.h>
#include <nlohmann/json_fwd.hpp>

#include <algorithm>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace hry
{
class KeyBinds;
class KeyBindsManager;
class KeyBindsPage;
class KeyBindBuilder;

/**
 * @brief Class for key binding
 *
 */
class KeyBind final
{
    friend KeyBindBuilder;

public:
    /**
     * @brief Delegate used to invoke press and release action
     */
    using Delegate_t = Delegate<void(ButtonState)>;

    /**
     * @brief Type of activator
     */
    enum class Activator
    {
        Click = 0,
        Hold
    };

public:
    /**
     * @brief Id of key bind for config file and internal use
     */
    std::string id;
    /**
     * @brief Name of the key bind, for display purposes
     */
    std::string label;
    /**
     * @brief Description of key bind, for display purposes
     */
    std::string desc;

    /**
     * @brief Default key
     *
     * Null for not set
     */
    const BindableKey* defaultKey = nullptr;
    /**
     * @brief Current assign key
     *
     * Null for not set
     */
    const BindableKey* key = nullptr;

    /**
     * @brief Activator type
     */
    Activator activator = Activator::Click;

    /**
     * @brief GUID of joystick, don't care if keyboard or mouse
     *
     * Set by internal system
     * Not recommended to change it
     */
    std::optional<GUID> joystickGUID;

    /**
     * @brief time point when key was pressed
     */
    std::chrono::system_clock::time_point keyPressTimePoint;

    /**
     * @brief Current button state
     */
    ButtonState state = ButtonState::Released;

    /**
     * @brief Delegate for press action
     */
    Delegate_t pressAction;
    /**
     * @brief Delegate for release action
     */
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
    /**
     * @brief Construct a new Key Binds object with name
     *
     * Use KeyBindBuilder to create bind
     *
     * @param name Name is used to identify section of keybinds, and seperate from others
     */
    explicit KeyBinds(std::string name);

    /**
     * @brief Register keybind
     *
     * @param keyBind Keybind to register
     * @return Registered keybind
     */
    HRY_API KeyBind* add(std::unique_ptr<KeyBind>&& keyBind);

    /**
     * @brief Save all keybind to config file
     */
    void saveToFile() const;

    /**
     * @brief Load keybinds from config file
     *
     * @return False if cannot open config file
     */
    bool loadFromFile();

private:
    const std::string& getName() const { return _name; }

    auto& getKeyBinds() { return _keyBinds; }
    const auto& getKeyBinds() const { return _keyBinds; }

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);
};

/**
 * @brief Use to create key binds
 */
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

    /**
     * required
     * @brief set identifier of key bind for config file
     *
     * @param id identifier of keybind
     */
    KeyBindBuilder& setID(const std::string& id)
    {
        _id = id;
        return *this;
    }

    /**
     * required
     * @brief Set the label, will be visible in menu
     *
     * @param label name to be displayed
     */
    KeyBindBuilder& setLabel(const std::string& label)
    {
        _label = label;
        return *this;
    }

    /**
     * optional
     * @brief Set description, will be shown next to the label in tooltip
     *
     * @param desc Description
     */
    KeyBindBuilder& setDescription(const std::string& desc)
    {
        _desc = desc;
        return *this;
    }

    /**
     * optional
     * @brief Set the Default Key
     *
     * @param key key to be default
     */
    KeyBindBuilder& setDefaultKey(BindableKey::Key_t key)
    {
        _defaultKey = key;
        return *this;
    }

    //
    /**
     * optional
     * @brief set how this keybind can be triggered
     *
     * Default is Activator::Click
     *
     * @param activator
     */
    KeyBindBuilder& setActivator(KeyBind::Activator activator)
    {
        _activator = activator;
        return *this;
    }

    /**
     * optional
     * @brief Set key press callback action
     *
     * @param callback Delegate to be invoke when key is pressed
     */
    KeyBindBuilder& setPressCallback(KeyBind::Delegate_t callback)
    {
        _pressAction = callback;
        return *this;
    }

    /**
     * optional
     * @brief Set key release callback action
     *
     * @param callback Delegate to be invoke when key is released
     */
    KeyBindBuilder& setReleaseCallback(KeyBind::Delegate_t callback)
    {
        _releaseAction = callback;
        return *this;
    }

    /**
     * required
     * @brief Create the key bind, pass it to add method in KeyBinds
     *
     * @return Constructed keybind object
     */
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
}
