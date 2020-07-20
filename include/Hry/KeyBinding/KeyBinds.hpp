#pragma once

#include <vector>
#include <variant>
#include <string>

#include "Hry/Export.hpp"
#include "Hry/System//Keyboard.hpp"
#include "Hry/System//Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

namespace hry
{

class KeyBinds;

class KeyBind
{
    friend KeyBinds;

public:
    using Key_t = std::variant<Keyboard::Key, Mouse::Button>; // TODO: Add controller's keys
    using Delegate_t = Delegate<void()>;

private:
    std::string _configFieldName;
    std::string _name;
    Key_t _key;
    Key_t _defaultKey;
    Delegate_t _pressAction;
    Delegate_t _releaseAction;

public:
    void setConfigFieldName(const char* name) { _configFieldName = name; }
    const auto& getConfigFieldName() const { return _configFieldName; }

    void setName(const char* name) { _name = name; }
    const auto& getName() const { return _name; }

    void setDefaultKey(Key_t key) { _key = key; _defaultKey = key; }
    const auto& getDefaultKey() const { return _key; }
    
    const auto& getKey() const { return _key; }

    template<auto FuncAddr>
    void setPressAction() { _pressAction.connect<FuncAddr>(); }

    template<auto CtxFuncAddr, typename T>
    void setPressAction(T* content) { _pressAction.connect<CtxFuncAddr>(content); }

    template<auto FuncAddr>
    void setReleaseAction() { _releaseAction.connect<FuncAddr>(); }

    template<auto CtxFuncAddr, typename T>
    void setReleaseAction(T* content) { _releaseAction.connect<CtxFuncAddr>(content); }
};

class KeyBinds
{
private:
    std::string _name;
    std::vector<KeyBind> _keyBinds;

public:
    KeyBinds(const std::string& name);

    void addKeyBind(KeyBind&& keyBind) { _keyBinds.push_back(std::move(keyBind)); }

    const std::string& getName() const { return _name; }

    const auto& getKeyBinds() const { return _keyBinds; }
}; 

}
