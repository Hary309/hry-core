/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/Config/Config.hpp"

#include <filesystem>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/Paths.hpp"

#include "Renderer/Renderer.hpp"
#include "Utils/Icons.hpp"

#include "Core.hpp"

namespace fs = std::filesystem;

namespace hry
{
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
        try
        {
            nlohmann::json json;
            toJson(json);
            f << json.dump(4);
            Core::Logger->info("Saved config for {}", _name);
        }
        catch (nlohmann::json::type_error& ex)
        {
            Core::Logger->error(
                "Cannot encode config for '{}' because '{}'", this->_name, ex.what());
        }
        catch (nlohmann::json::exception& ex)
        {
            Core::Logger->error("Cannot save config for '{}' because '{}'", this->_name, ex.what());
        }
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
        try
        {
            nlohmann::json json;
            f >> json;
            fromJson(json);
            Core::Logger->info("Loaded config for {}", _name);
            invokeCallback();

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

    // load default configs
    invokeCallback();

    return false;
}

bool Config::isDirty() const
{
    bool isDirty = false;

    for (const auto& field : _fields)
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
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnOffset(1, 10);
    ImGui::SetColumnOffset(2, ImGui::GetWindowContentRegionWidth() - 32);

    for (auto& field : _fields)
    {
        ImGui::PushID(&field);

        if (field->isDirty())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor(50, 200, 50).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 200, 50).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(50, 200, 50).Value);
            ImGui::Button("", { 1, 0 });
            ImGui::PopStyleColor(3);
        }

        ImGui::NextColumn();

        field->imguiRender();

        ImGui::NextColumn();

        // show `Reset to default` button only when dirty value is different from the default
        if (field->canResetToDefault())
        {
            if (ImGui::Button(Icons::Undo))
            {
                field->resetToDefault();
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("Set default value");
                ImGui::EndTooltip();
            }
        }

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::Columns(1);
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
        HryPtr<void> data{ _bindingStructCtor(), { _bindingStructDtor, nullptr } };

        ConfigCallbackData callbackData{ data.get(), _bindingStructSize };

        for (auto& field : _fields)
        {
            field->setupCallbackData(callbackData);
        }

        onChangesApplied(callbackData);
    }
}
}
