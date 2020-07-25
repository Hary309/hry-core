#include "Hry/Config/Fields/CheckboxField.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void CheckboxField::imguiRender()
{
    if (ImGui::Checkbox(_label.c_str(), &_dirtyValue))
    {
        onValueChange.call(_dirtyValue);
    }

    ImGui::SameLine();

    if (!_description.empty())
    {
        ImGuiHelpMarker(_description.c_str());
    }
}

void CheckboxField::save(nlohmann::json& json)
{
    json[_configFieldName] = _value;
}

void CheckboxField::load(const nlohmann::json& json)
{
    if (auto it = json.find(_configFieldName); it != json.end())
    {
        _value = it->get<bool>();
        _dirtyValue = _value;
    }
}

HRY_NS_END
