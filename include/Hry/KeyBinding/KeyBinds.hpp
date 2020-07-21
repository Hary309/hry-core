#pragma once

#include <algorithm>
#include <vector>
#include <variant>
#include <string>

#include "BindableKeys.hpp"

#include "Hry/Export.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/Utils/Delegate.hpp"

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
    const BindableKey* _key = nullptr; // if null no set
    const BindableKey* _defaultKey = nullptr; // if null not set
    Delegate_t _pressAction;
    Delegate_t _releaseAction;

    bool _isKeyPressed = false;

public:
    void setConfigFieldName(const char* name) { _configFieldName = name; }
    const auto& getConfigFieldName() const { return _configFieldName; }

    void setName(const char* name) { _name = name; }
    const auto& getName() const { return _name; }

    void setDefaultKey(const BindableKey* key) { _key = key; _defaultKey = key; }
    void setDefaultKey(const BindableKey::Key_t key)
    {
        auto it = std::find_if(BindableKeys.begin(), BindableKeys.end(),
            [&key](const BindableKey& bindableKey)
                { return key == bindableKey.key; }
        );

        if (it != BindableKeys.end())
        {
            setDefaultKey(&*it);
        }
    }
    const auto& getDefaultKey() const { return _key; }
    
    const auto& getKey() const { return _key; }

    void setKeyPressState(bool isPressed) { _isKeyPressed = isPressed; }
    bool isKeyPressState() const { return _isKeyPressed; }

    void callPressAction() const { _pressAction.call(); }

    template<auto FuncAddr>
    void setPressAction() { _pressAction.connect<FuncAddr>(); }

    template<auto CtxFuncAddr, typename T>
    void setPressAction(T* content) { _pressAction.connect<CtxFuncAddr>(content); }

    void callReleaseAction() const { _releaseAction.call(); }

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

    auto& getKeyBinds() { return _keyBinds; }
    const auto& getKeyBinds() const { return _keyBinds; }
}; 

}
