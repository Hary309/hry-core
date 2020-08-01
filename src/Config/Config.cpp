#include "Hry/Config/Config.hpp"

#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

Config::Config(std::string name) : _name(std::move(name))
{
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
        field->imguiRender();
    }
}

void Config::toJson(nlohmann::json& json)
{
    for (auto& field : _fields)
    {
        field->toJson(json);
    }
}

void Config::fromJson(const nlohmann::json& json)
{
    for (auto& field : _fields)
    {
        field->fromJson(json);
    }
}

void Config::invokeCallback()
{
    ConfigCallbackData callbackData;

    for (auto& field : _fields)
    {
        field->setupCallbackData(callbackData);
    }

    onChangesApplied(std::move(callbackData));
}

HRY_NS_END
