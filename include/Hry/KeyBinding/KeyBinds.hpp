#pragma once

#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

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

    enum class TriggerType
    {
        Click = 0,
        Hold
    };

private:
    std::string _configFieldName;
    std::string _name;

    const BindableKey* _defaultKey = nullptr; // if null not set
    const BindableKey* _key = nullptr;        // if null no set

    TriggerType _defaultTriggerType = TriggerType::Click;
    TriggerType _triggerType = TriggerType::Click;

    // time point when key was pressed
    std::chrono::system_clock::time_point _keyPressTimePoint;

    ButtonState _state = ButtonState::Released;

public:
    Delegate_t pressAction;
    Delegate_t releaseAction;

public:
    KeyBind() = default;
    explicit KeyBind(TriggerType triggerType)
        : _defaultTriggerType(triggerType), _triggerType(triggerType)
    {
    }

    void setConfigFieldName(const char* name);

    void setName(const char* name);

    void setDefaultKey(const BindableKey* key);
    void setDefaultKey(BindableKey::Key_t key);

    // default value is TriggerType::Click
    void setDefaultTriggerType(TriggerType triggerType);

private:
    auto getConfigFieldName() const -> const std::string&;
    auto getName() const -> const std::string&;

    void setKey(const BindableKey* key);
    void setKey(BindableKey::Key_t key);
    auto getKey() const -> const BindableKey*;

    auto getDefaultKey() const -> const BindableKey*;
    auto getTriggerType() const -> TriggerType;
};

class KeyBinds
{
private:
    std::string _name;
    std::vector<KeyBind> _keyBinds;

public:
    explicit KeyBinds(std::string name) : _name(std::move(name)) {}

    void addKeyBind(KeyBind&& keyBind) { _keyBinds.push_back(std::move(keyBind)); }

    [[nodiscard]] const std::string& getName() const { return _name; }

    auto& getKeyBinds() { return _keyBinds; }
    [[nodiscard]] const auto& getKeyBinds() const { return _keyBinds; }

    HRY_API void toJson(nlohmann::json& json);
    HRY_API void fromJson(const nlohmann::json& json);
};

inline auto KeyBind::setConfigFieldName(const char* name) -> void
{
    _configFieldName = name;
}

inline auto KeyBind::getConfigFieldName() const -> const std::string&
{
    return _configFieldName;
}

inline auto KeyBind::setName(const char* name) -> void
{
    _name = name;
}

inline auto KeyBind::getName() const -> const std::string&
{
    return _name;
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

inline void KeyBind::setDefaultTriggerType(TriggerType triggerType)
{
    _defaultTriggerType = triggerType;
    _triggerType = triggerType;
}

inline auto KeyBind::getTriggerType() const -> TriggerType
{
    return _triggerType;
}

HRY_NS_END
