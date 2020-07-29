#include "Hry/Config/Fields/SelectionField.hpp"

#include <type_traits>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

void SelectionField::imguiRender()
{
    int size = _options.size();

    if (size > 0)
    {
        if (_dirtySelectedIndex > size)
        {
            _dirtySelectedIndex = 0;
        }

        std::visit([this, &size](auto&& arg) { renderWidget(arg, size); }, _type);

        if (!_description.empty())
        {
            ImGui::SameLine();
            ImGuiHelpMarker(_description.c_str());
        }
    }
}

void SelectionField::toJson(nlohmann::json& json)
{
    json[_configFieldName] = _options[_selectedIndex];
}

void SelectionField::fromJson(const nlohmann::json& json)
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

void SelectionField::renderWidget(ComboType& /*unused*/, int size)
{
    if (ImGui::BeginCombo(_label.c_str(), _options[_dirtySelectedIndex].c_str()))
    {
        for (int i = 0; i < size; i++)
        {
            auto& option = _options[i];

            bool isSelected = _dirtySelectedIndex == i;

            if (ImGui::Selectable(option.c_str(), isSelected))
            {
                _dirtySelectedIndex = i;
                onPreviewChange(_dirtySelectedIndex);
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}

void SelectionField::renderWidget(RadioType& radio, int size)
{
    for (int i = 0; i < size; i++)
    {
        auto& option = _options[i];
        if (ImGui::RadioButton(option.c_str(), i == _dirtySelectedIndex))
        {
            _dirtySelectedIndex = i;
            onPreviewChange(_dirtySelectedIndex);
        }

        if (radio.sameLine && i != size - 1)
        {
            ImGui::SameLine();
        }
    }
}

HRY_NS_END
