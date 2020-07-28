#pragma once

#include <algorithm>
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

#include "BindableKeys.hpp"

HRY_NS_BEGIN

class KeyBinds;

class KeyBind
{
public:
    using Delegate_t = Delegate<void()>;

private:
    std::string _configFieldName;
    std::string _name;
    const BindableKey* _defaultKey = nullptr; // if null not set
    const BindableKey* _key = nullptr;        // if null no set

    ButtonState _state = ButtonState::Released;

public:
    Delegate_t pressAction;
    Delegate_t releaseAction;

public:
    auto setConfigFieldName(const char* name) -> void;
    [[nodiscard]] auto getConfigFieldName() const -> const std::string&;

    auto setName(const char* name) -> void;
    [[nodiscard]] auto getName() const -> const std::string&;

    auto setDefaultKey(const BindableKey* key) -> void;
    auto setDefaultKey(BindableKey::Key_t key) -> void;
    [[nodiscard]] auto getDefaultKey() const -> const BindableKey*;

    auto setKey(const BindableKey* key) -> void;
    auto setKey(BindableKey::Key_t key) -> void;
    [[nodiscard]] auto getKey() const -> const BindableKey*;

    auto setKeyState(ButtonState state) -> void;
    [[nodiscard]] auto getKeyState() const -> ButtonState;
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
    auto it = std::find_if(
        BindableKeys.begin(), BindableKeys.end(),
        [&key](const BindableKey& bindableKey) { return key == bindableKey.key; });

    if (it != BindableKeys.end())
    {
        setDefaultKey(&*it);
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
    auto it = std::find_if(
        BindableKeys.begin(), BindableKeys.end(),
        [&key](const BindableKey& bindableKey) { return key == bindableKey.key; });

    if (it != BindableKeys.end())
    {
        setKey(&*it);
    }
}

inline auto KeyBind::setKeyState(ButtonState state) -> void
{
    _state = state;
}

inline auto KeyBind::getKeyState() const -> ButtonState
{
    return _state;
}

HRY_NS_END
