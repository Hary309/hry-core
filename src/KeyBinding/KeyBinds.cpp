#include "Hry/KeyBinding/KeyBinds.hpp"

#include <string>

#include <nlohmann/json.hpp>

#include "Hry/System/Mouse.hpp"

HRY_NS_BEGIN

HRY_API void KeyBinds::toJson(nlohmann::json& json)
{
    for (auto& keyBind : _keyBinds)
    {
        const auto* key = keyBind.getKey();

        if (key == nullptr)
        {
            continue;
        }

        if (key->has<Keyboard::Key>())
        {
            json[keyBind.getConfigFieldName()] = {
                { "device", "keyboard" },
                { "trigger", static_cast<int>(keyBind._triggerType) },
                { "key", static_cast<int>(std::get<Keyboard::Key>(key->key)) }
            };
        }
        else if (key->has<Mouse::Button>())
        {
            json[keyBind.getConfigFieldName()] = {
                { "device", "mouse" },
                { "trigger", static_cast<int>(keyBind._triggerType) },
                { "key", static_cast<int>(std::get<Mouse::Button>(key->key)) }
            };
        }
    }
}

HRY_API void KeyBinds::fromJson(const nlohmann::json& json)
{
    for (auto& keyBind : _keyBinds)
    {
        auto jKeyBind = json.find(keyBind.getConfigFieldName());

        if (jKeyBind != json.end())
        {
            auto jDevice = jKeyBind->find("device");

            if (jDevice == jKeyBind->end())
            {
                continue;
            }

            auto jKey = jKeyBind->find("key");

            if (jKey == jKeyBind->end())
            {
                continue;
            }

            auto deviceType = jDevice->get<std::string>();

            if (auto jTrigger = jKeyBind->find("trigger"); jTrigger != jKeyBind->end())
            {
                keyBind._triggerType = static_cast<KeyBind::TriggerType>(jTrigger->get<int>());
            }

            if (deviceType == "keyboard")
            {
                auto key = static_cast<Keyboard::Key>(jKey->get<int>());
                keyBind.setKey(key);
            }
            else if (deviceType == "mouse")
            {
                auto key = static_cast<Mouse::Button>(jKey->get<int>());
                keyBind.setKey(key);
            }
        }
    }
}

HRY_NS_END