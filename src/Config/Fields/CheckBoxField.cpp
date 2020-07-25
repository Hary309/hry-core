#include "Hry/Config/Fields/CheckBoxField.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void CheckBoxField::imguiRender()
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

void CheckBoxField::save(nlohmann::json& json)
{
    json[_configFieldName] = _value;
}

void CheckBoxField::load(const nlohmann::json& json)
{
    if (auto it = json.find(_configFieldName); it != json.end())
    {
        _value = it->get<bool>();
        _dirtyValue = _value;
    }
}

HRY_NS_END
