#include "Hry/Config/Config.hpp"

#include <filesystem>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/Paths.hpp"

#include "Renderer/Renderer.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

HRY_NS_BEGIN

Config::Config(std::string name) : _name(std::move(name))
{
    _configFilePath = fmt::format("{}\\{}.json", Paths::ConfigsPath, _name);
}

void Config::saveToFile() const
{
    if (!fs::exists(Paths::ConfigsPath))
    {
        fs::create_directories(Paths::ConfigsPath);
    }

    std::ofstream f(_configFilePath);

    if (f.is_open())
    {
        nlohmann::json json;
        toJson(json);
        f << json.dump(4);
        Core::Logger->info("Saved config for {}", _name);
    }
    else
    {
        Core::Logger->error("Cannot save config to {}", _configFilePath);
    }
}

bool Config::loadFromFile()
{
    std::ifstream f(_configFilePath);

    if (f.is_open())
    {
        nlohmann::json json;
        f >> json;
        fromJson(json);
        Core::Logger->info("Loaded config for {}", _name);
        invokeCallback();

        return true;
    }

    return false;
}

bool Config::isDirty()
{
    bool isDirty = false;

    for (auto& field : _fields)
    {
        isDirty |= field->isDirty();
    }

    return isDirty;
}

void Config::applyChanges()
{
    if (isDirty())
    {
        for (auto& field : _fields)
        {
            field->applyChanges();
        }

        invokeCallback();
    }
}

void Config::cancelChanges()
{
    for (auto& field : _fields)
    {
        field->cancelChanges();
    }
}

void Config::resetToDefault()
{
    for (auto& field : _fields)
    {
        field->resetToDefault();
    }
}

void Config::imguiRender()
{
    for (auto& field : _fields)
    {
        ImGui::PushID(&field);
        field->imguiRender();
        ImGui::PopID();
    }
}

void Config::toJson(nlohmann::json& json) const
{
    for (const auto& field : _fields)
    {
        field->toJson(json);
    }

    auto extraData = nlohmann::json::object();
    onSave(extraData);
    json["extra"] = extraData;
}

void Config::fromJson(const nlohmann::json& json)
{
    for (auto& field : _fields)
    {
        field->fromJson(json);
    }

    if (auto it = json.find("extra"); it != json.end())
    {
        onLoad(it.value());
    }
}

void Config::invokeCallback()
{
    if (_bindingStructSize > 0)
    {
        ConfigCallbackData callbackData{ _bindingStructSize };

        for (auto& field : _fields)
        {
            field->setupCallbackData(callbackData);
        }

        onChangesApplied(callbackData);
        _bindingStructDtor(callbackData._data.data());
    }
}

HRY_NS_END
