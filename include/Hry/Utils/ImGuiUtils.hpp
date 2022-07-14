/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <imgui.h>

#include <cstdint>

namespace hry
{
/**
 * @brief Renders helper marker in ImGui Window
 *
 * @param desc Text to render
 */
inline void ImGuiHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

/**
 * @brief Converts type to Dear ImGui data type
 *
 * @tparam Type to convert
 */
template<typename>
struct ImGuiDataType;

template<>
struct ImGuiDataType<int8_t>
{
    static constexpr int Value = ImGuiDataType_S8;
};

template<>
struct ImGuiDataType<uint8_t>
{
    static constexpr int Value = ImGuiDataType_S8;
};

template<>
struct ImGuiDataType<int16_t>
{
    static constexpr int Value = ImGuiDataType_S16;
};

template<>
struct ImGuiDataType<uint16_t>
{
    static constexpr int Value = ImGuiDataType_U16;
};

template<>
struct ImGuiDataType<int32_t>
{
    static constexpr int Value = ImGuiDataType_S32;
};

template<>
struct ImGuiDataType<uint32_t>
{
    static constexpr int Value = ImGuiDataType_U32;
};

template<>
struct ImGuiDataType<int64_t>
{
    static constexpr int Value = ImGuiDataType_S64;
};

template<>
struct ImGuiDataType<uint64_t>
{
    static constexpr int Value = ImGuiDataType_U64;
};

template<>
struct ImGuiDataType<float>
{
    static constexpr int Value = ImGuiDataType_Float;
};

template<>
struct ImGuiDataType<double>
{
    static constexpr int Value = ImGuiDataType_Double;
};

/**
 * @brief Converts type to Dear ImGui data type
 *
 * @tparam T Type to convert
 */
template<typename T>
static inline constexpr auto ImGuiDataType_v = ImGuiDataType<T>::Value;
}
