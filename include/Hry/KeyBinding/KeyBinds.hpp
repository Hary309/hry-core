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

class KeyBind
{
    friend KeyBinds;
    friend KeyBindsManager;
    friend KeyBindsPage;

public:
    using Delegate_t = Delegate<void(ButtonState)>;

    enum class Activator
    {
        Click = 0,
        Hold
    };

private:
    std::string _configFieldName;
    std::string _label;

    const BindableKey* _defaultKey = nullptr; // if null not set
    const BindableKey* _key = nullptr;        // if null no set

    Activator _defaultActivator = Activator::Click;
    Activator _activator = Activator::Click;

    std::optional<GUID> _joystickGUID;

    // time point when key was pressed
    std::chrono::system_clock::time_point _keyPressTimePoint;

    ButtonState _state = ButtonState::Released;

public:
    Delegate_t pressAction;
    Delegate_t releaseAction;

public:
    KeyBind() = default;
    explicit KeyBind(Activator activator) : _defaultActivator(activator), _activator(activator) {}

    void setDefaultKey(const BindableKey* key);
    void setDefaultKey(BindableKey::Key_t key);

    // default value is Activator::Click
    void setDefaultActivator(Activator activator);

private:
    auto getConfigFieldName() const -> const std::string&;
    auto getName() const -> const std::string&;

    void setKey(const BindableKey* key);
    void setKey(BindableKey::Key_t key);
    auto getKey() const -> const BindableKey*;

    auto getDefaultKey() const -> const BindableKey*;
    auto getActivator() const -> Activator;

    void setJoystickGUID(GUID guid);
    auto getJoystickGUID() const -> std::optional<GUID>;

    void setKeyPressTimePoint(std::chrono::system_clock::time_point timePoint);
    auto getKeyPressTimePoint() const -> std::chrono::system_clock::time_point;

    auto setState(ButtonState state);
    auto getState() const -> ButtonState;
};

class KeyBinds
{
private:
    std::string _label;
    std::vector<std::unique_ptr<KeyBind>> _keyBinds;

public:
    explicit KeyBinds(std::string name) : _label(std::move(name)) {}

    KeyBind* createKeyBind(std::string label, std::string configFieldName)
    {
        auto* keyBind = new KeyBind();
        keyBind->_label = std::move(label);
        keyBind->_configFieldName = std::move(configFieldName);
        _keyBinds.push_back(std::unique_ptr<KeyBind>(keyBind));

        return keyBind;
    }

    [[nodiscard]] const std::string& getName() const { return _label; }

    auto& getKeyBinds() { return _keyBinds; }
    [[nodiscard]] const auto& getKeyBinds() const { return _keyBinds; }

    HRY_API void toJson(nlohmann::json& json);
    HRY_API void fromJson(const nlohmann::json& json);
};

inline auto KeyBind::getConfigFieldName() const -> const std::string&
{
    return _configFieldName;
}

inline auto KeyBind::getName() const -> const std::string&
{
    return _label;
}

inline void KeyBind::setDefaultKey(const BindableKey* key)
{
    _key = key;
    _defaultKey = key;
}

inline auto KeyBind::getDefaultKey() const -> const BindableKey*
{
    return _defaultKey;
}

inline void KeyBind::setDefaultKey(BindableKey::Key_t key)
{
    const auto* bindableKey = GetBindableKey(key);

    if (bindableKey != nullptr)
    {
        setDefaultKey(bindableKey);
    }
}

inline auto KeyBind::getKey() const -> const BindableKey*
{
    return _key;
}

inline auto KeyBind::setKey(const BindableKey* key) -> void
{
    _key = key;
}

inline void KeyBind::setKey(BindableKey::Key_t key)
{
    const auto* bindableKey = GetBindableKey(key);

    if (bindableKey != nullptr)
    {
        setKey(bindableKey);
    }
}

inline void KeyBind::setDefaultActivator(Activator activator)
{
    _defaultActivator = activator;
    _activator = activator;
}

inline auto KeyBind::getActivator() const -> Activator
{
    return _activator;
}

inline void KeyBind::setJoystickGUID(GUID guid)
{
    _joystickGUID = std::move(guid);
}

inline auto KeyBind::getJoystickGUID() const -> std::optional<GUID>
{
    return _joystickGUID;
}

inline void KeyBind::setKeyPressTimePoint(std::chrono::system_clock::time_point timePoint)
{
    _keyPressTimePoint = timePoint;
}

inline auto KeyBind::getKeyPressTimePoint() const -> std::chrono::system_clock::time_point
{
    return _keyPressTimePoint;
}

inline auto KeyBind::setState(ButtonState state)
{
    _state = state;
}

inline auto KeyBind::getState() const -> ButtonState
{
    return _state;
}

HRY_NS_END
