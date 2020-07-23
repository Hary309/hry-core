#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Hry/Export.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "BindableKeys.hpp"

namespace hry
{
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
    auto getConfigFieldName() const -> const std::string&;

    auto setName(const char* name) -> void;
    auto getName() const -> const std::string&;

    auto setDefaultKey(const BindableKey* key) -> void;
    auto setDefaultKey(const BindableKey::Key_t key) -> void;
    auto getDefaultKey() const -> const BindableKey*;

    auto setKey(const BindableKey* key) -> void;
    auto setKey(const BindableKey::Key_t key) -> void;
    auto getKey() const -> const BindableKey*;

    auto setKeyState(ButtonState state) -> void;
    auto getKeyState() const -> ButtonState;
};

class KeyBinds
{
private:
    std::string _name;
    std::vector<KeyBind> _keyBinds;

public:
    KeyBinds(const std::string& name) : _name(name) {}

    void addKeyBind(KeyBind&& keyBind) { _keyBinds.push_back(std::move(keyBind)); }

    const std::string& getName() const { return _name; }

    auto& getKeyBinds() { return _keyBinds; }
    const auto& getKeyBinds() const { return _keyBinds; }
};

using KeyBindsUniquePtr_t = std::unique_ptr<KeyBinds, Delegate<void(KeyBinds*)>>;

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

inline void KeyBind::setDefaultKey(const BindableKey::Key_t key)
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

inline void KeyBind::setKey(const BindableKey::Key_t key)
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

} // namespace hry
