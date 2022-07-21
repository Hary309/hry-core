/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/AxisBinding/AxisBinds.hpp"
#include "Core.hpp"
#include "Hry/Utils/Paths.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <utility>

namespace fs = std::filesystem;

namespace hry
{
AxisBinds::AxisBinds(std::string name)
    : _name(std::move(name))
{
    _axisBindsFilePath = fmt::format("{}/{}.json", Paths::AxisBindsPath, _name);
}

void AxisBinds::add(std::unique_ptr<AxisBind>&& axisBind)
{
    _axisBinds.push_back(std::move(axisBind));
}

void AxisBinds::saveToFile() const
{
    if (!fs::exists(Paths::AxisBindsPath))
    {
        fs::create_directory(Paths::AxisBindsPath);
    }

    std::ofstream f(_axisBindsFilePath);

    if (f.is_open())
    {
        try
        {
            nlohmann::json json;
            toJson(json);

            f << json.dump(4);

            Core::Logger->info("Saved axisbinds for {}", _name);
        }
        catch (nlohmann::json::type_error& ex)
        {
            Core::Logger->error("Cannot encode config for '{}' because '{}'", this->_name, ex.what());
        }
        catch (nlohmann::json::exception& ex)
        {
            Core::Logger->error("Cannot save config for '{}' because '{}'", this->_name, ex.what());
        }
    }
    else
    {
        Core::Logger->error("Cannot save axisbinds to {}", _axisBindsFilePath);
    }
}

bool AxisBinds::loadFromFile()
{
    std::ifstream f(_axisBindsFilePath);

    if (f.is_open())
    {
        try
        {
            nlohmann::json json;
            f >> json;
            fromJson(json);
            Core::Logger->info("Loaded axisbinds for {}", _name);

            return true;
        }
        catch (nlohmann::json::parse_error& ex)
        {
            Core::Logger->error(
                "Cannot parse config for '{}' because '{}'", this->_name, ex.what());
        }
        catch (nlohmann::json::exception& ex)
        {
            Core::Logger->error("Cannot load config for '{}' because '{}'", this->_name, ex.what());
        }
    }

    return false;
}

void AxisBinds::toJson(nlohmann::json& json) const
{
    for (const auto& axisBind : _axisBinds)
    {
        if (!axisBind->axis.has_value() || !axisBind->deviceGUID.has_value())
        {
            continue;
        }

        const auto& id = axisBind->id;
        auto& jObject = json[id];

        jObject["axis"] = static_cast<int>(axisBind->axis.value());
        jObject["dead_zone"] = axisBind->deadZone;

        auto& guid = axisBind->deviceGUID;
        jObject["guid"] = { { "data1", guid->Data1 },
                            { "data2", guid->Data2 },
                            { "data3", guid->Data3 },
                            { "data4", *reinterpret_cast<uint64_t*>(guid->Data4) } };
    }
}

void AxisBinds::fromJson(const nlohmann::json& json)
{
    for (auto& axisBind : _axisBinds)
    {
        auto jAxisBind = json.find(axisBind->id);

        if (jAxisBind != json.end())
        {
            auto jAxis = jAxisBind->find("axis");

            if (jAxis == jAxisBind->end())
            {
                continue;
            }

            auto jDeadZone = jAxisBind->find("dead_zone");

            if (jDeadZone == jAxisBind->end())
            {
                continue;
            }

            auto jGUID = jAxisBind->find("guid");

            if (jGUID == jAxisBind->end())
            {
                continue;
            }

            GUID guid;
            guid.Data1 = jGUID->find("data1")->get<uint32_t>();
            guid.Data2 = jGUID->find("data2")->get<uint16_t>();
            guid.Data3 = jGUID->find("data3")->get<uint16_t>();
            auto data4 = jGUID->find("data4")->get<uint64_t>();

            memcpy(guid.Data4, &data4, sizeof(uint64_t));

            axisBind->axis = { static_cast<Joystick::Axis>(jAxis->get<int>()) };
            axisBind->deadZone = jDeadZone->get<float>();
            axisBind->deviceGUID = guid;
        }
    }
}
}
