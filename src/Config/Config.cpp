#include "Hry/Config/Config.hpp"

#include "Hry/Config/ConfigFieldBase.hpp"

HRY_NS_BEGIN

Config::Config(const std::string& name) : _name(name)
{
}

void Config::imguiRender()
{
    for (auto& field : _fields)
    {
        field->imguiRender();
    }
}

void Config::save()
{
}

void Config::load()
{
}

HRY_NS_END
