#include "Hry/KeyBinding/KeyBinds.hpp"

#include <string>

#include <nlohmann/json.hpp>

#include "Hry/System/Joystick.hpp"
#include "Hry/System/Mouse.hpp"

HRY_NS_BEGIN

HRY_API void KeyBinds::toJson(nlohmann::json& json)
{
    for (auto& keyBind : _keyBinds)
    {
        const auto* key = keyBind->getKey();

        if (key == nullptr)
        {
            continue;
        }

        const auto& configFieldName = keyBind->getConfigFieldName();
        auto& jObject = json[configFieldName];
        jObject["trigger"] = static_cast<int>(keyBind->getActivator());

        if (key->has<Keyboard::Key>())
        {
            jObject["device"] = "keyboard";
            jObject["key"] = static_cast<int>(std::get<Keyboard::Key>(key->key));
        }
        else if (key->has<Mouse::Button>())
        {
            jObject["device"] = "mouse";
            jObject["key"] = static_cast<int>(std::get<Mouse::Button>(key->key));
        }
        else if (key->has<Joystick::Button>())
        {
            jObject["device"] = "joystick";
            jObject["key"] = static_cast<int>(std::get<Joystick::Button>(key->key));

            auto& guid = keyBind->_joystickGUID;
            jObject["guid"] = { { "data1", guid->Data1 },
                                { "data2", guid->Data2 },
                                { "data3", guid->Data3 },
                                { "data4", *reinterpret_cast<uint64_t*>(guid->Data4) } };
        }
    }
}

HRY_API void KeyBinds::fromJson(const nlohmann::json& json)
{
    for (auto& keyBind : _keyBinds)
    {
        auto jKeyBind = json.find(keyBind->getConfigFieldName());

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

            const auto keyId = jKey->get<int>();

            auto deviceType = jDevice->get<std::string>();

            if (auto jTrigger = jKeyBind->find("trigger"); jTrigger != jKeyBind->end())
            {
                keyBind->_activator = static_cast<KeyBind::Activator>(jTrigger->get<int>());
            }

            if (deviceType == "keyboard")
            {
                auto key = static_cast<Keyboard::Key>(keyId);
                keyBind->setKey(key);
            }
            else if (deviceType == "mouse")
            {
                auto key = static_cast<Mouse::Button>(keyId);
                keyBind->setKey(key);
            }
            else if (deviceType == "joystick")
            {
                auto key = static_cast<Joystick::Button>(keyId);
                keyBind->setKey(key);

                auto jGUID = jKeyBind->find("guid");

                if (jGUID == jKeyBind->end())
                {
                    continue;
                }

                GUID guid;
                guid.Data1 = jGUID->find("data1")->get<uint32_t>();
                guid.Data2 = jGUID->find("data2")->get<uint16_t>();
                guid.Data3 = jGUID->find("data3")->get<uint16_t>();
                auto data4 = jGUID->find("data4")->get<uint64_t>();

                memcpy(guid.Data4, &data4, sizeof(uint64_t));

                keyBind->setJoystickGUID(guid);
            }
        }
    }
}

HRY_NS_END