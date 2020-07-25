#include "Hry/Config/Fields/ComboBoxField.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void ComboBoxField::imguiRender()
{
    auto size = _options.size();

    if (size > 0)
    {
        if (_dirtySelectedIndex > size)
            _dirtySelectedIndex = 0;

        if (ImGui::BeginCombo(_label.c_str(), _options[_dirtySelectedIndex].c_str()))
        {
            for (size_t i = 0; i < size; i++)
            {
                auto& option = _options[i];

                bool isSelected = _dirtySelectedIndex == i;

                if (ImGui::Selectable(option.c_str(), isSelected))
                {
                    _dirtySelectedIndex = i;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                    onValueChange(_dirtySelectedIndex);
                }
            }

            ImGui::EndCombo();
        }

        if (!_description.empty())
        {
            ImGui::SameLine();
            ImGuiHelpMarker(_description.c_str());
        }
    }
}

void ComboBoxField::save(nlohmann::json& json)
{
    json[_configFieldName] = _options[_selectedIndex];
}

void ComboBoxField::load(const nlohmann::json& json)
{
    auto it = json.find(_configFieldName);

    if (it != json.end())
    {
        auto index = getIndex(it->get<std::string>());

        if (index != -1)
        {
            _selectedIndex = index;
            _dirtySelectedIndex = index;
        }
    }
}

HRY_NS_END
