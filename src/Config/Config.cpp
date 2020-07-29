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

void Config::imguiRender()
{
    for (auto& field : _fields)
    {
        field->imguiRender();
    }
}

void Config::toJson(nlohmann::json& json)
{
    json["name"] = _name;

    auto fields = json["fields"];

    for (auto& field : _fields)
    {
        field->toJson(fields);
    }
}

void Config::fromJson(const nlohmann::json& json)
{
    for (auto& field : _fields)
    {
        field->fromJson(json);
    }
}

HRY_NS_END
