/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/KeyBinding/KeyBinds.hpp"

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/Utils/Paths.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

HRY_NS_BEGIN

KeyBinds::KeyBinds(std::string name) : _name(std::move(name))
{
    _keyBindsFilePath = fmt::format("{}/{}.json", Paths::KeyBindsPath, _name);
}

KeyBind* KeyBinds::add(std::unique_ptr<KeyBind>&& keyBind)
{
    auto* ptr = keyBind.get();

    _keyBinds.push_back(std::move(keyBind));

    return ptr;
}

void KeyBinds::saveToFile() const
{
    if (!fs::exists(Paths::KeyBindsPath))
    {
        fs::create_directory(Paths::KeyBindsPath);
    }

    std::ofstream f(_keyBindsFilePath);

    if (f.is_open())
    {
        nlohmann::json json;
        toJson(json);

        f << json.dump(4);

        Core::Logger->info("Saved keybinds for {}", _name);
    }
    else
    {
        Core::Logger->error("Cannot save keybinds to {}", _keyBindsFilePath);
    }
}

bool KeyBinds::loadFromFile()
{
    std::ifstream f(_keyBindsFilePath);

    if (f.is_open())
    {
        nlohmann::json json;
        f >> json;
        fromJson(json);
        Core::Logger->info("Loaded keybinds for {}", _name);

        return true;
    }

    return false;
}

void KeyBinds::toJson(nlohmann::json& json) const
{
    for (const auto& keyBind : _keyBinds)
    {
        const auto* key = keyBind->key;

        if (key == nullptr)
        {
            continue;
        }

        const auto& id = keyBind->id;
        auto& jObject = json[id];
        jObject["trigger"] = static_cast<int>(keyBind->activator);

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

            auto& guid = keyBind->joystickGUID;
            jObject["guid"] = { { "data1", guid->Data1 },
                                { "data2", guid->Data2 },
                                { "data3", guid->Data3 },
                                { "data4", *reinterpret_cast<uint64_t*>(guid->Data4) } };
        }
    }
}

void KeyBinds::fromJson(const nlohmann::json& json)
{
    for (auto& keyBind : _keyBinds)
    {
        auto jKeyBind = json.find(keyBind->id);

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
                keyBind->activator = static_cast<KeyBind::Activator>(jTrigger->get<int>());
            }

            if (deviceType == "keyboard")
            {
                auto key = static_cast<Keyboard::Key>(keyId);
                keyBind->key = GetBindableKey(key);
            }
            else if (deviceType == "mouse")
            {
                auto key = static_cast<Mouse::Button>(keyId);
                keyBind->key = GetBindableKey(key);
            }
            else if (deviceType == "joystick")
            {
                auto key = static_cast<Joystick::Button>(keyId);
                keyBind->key = GetBindableKey(key);

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

                keyBind->joystickGUID = guid;
            }
        }
    }
}

HRY_NS_END