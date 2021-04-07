/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "AboutPage.hpp"

#include <string_view>

#include <Windows.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <imgui.h>
#include <shellapi.h>

#include "Hry/Colors.hpp"
#include "Hry/Fonts.hpp"
#include "Hry/Version.hpp"

#include "UI/Markdown.hpp"
#include "Utils/Icons.hpp"

HRY_NS_BEGIN

void TextCentered(const char* text)
{
    ImGui::Dummy({ ImGui::GetWindowContentRegionWidth() / 2 - ImGui::CalcTextSize(text).x / 2, 1 });
    ImGui::SameLine();
    ImGui::Text("%s", text);
}

void AboutPage::imguiRender()
{
    ImGui::PushFont(Fonts::OpenSans_Bold_20);
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::Orange.Value);
    TextCentered(fmt::format("hry-core v{}", ApiVersion).c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();

    TextCentered("Piotr Krupa <piotrkrupa06@gmail.com>");

    ImGui::Columns(3, "AboutPage##Columns", false);

    if (ImGui::Button(
            fmt::format(FMT_COMPILE("Buy me a Coffee {}"), Icons::ExternalLink).c_str(),
            { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "https://ko-fi.com/hary309", nullptr, nullptr, SW_SHOWNORMAL);
    }

    ImGui::NextColumn();

    if (ImGui::Button(
            fmt::format(FMT_COMPILE("Website {}"), Icons::ExternalLink).c_str(), { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "https://hary309.github.io/projects/hry-plugins/", nullptr, nullptr,
            SW_SHOWNORMAL);
    }

    ImGui::NextColumn();

    if (ImGui::Button(
            fmt::format(FMT_COMPILE("Github {}"), Icons::ExternalLink).c_str(), { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "https://github.com/Hary309/hry-core", nullptr, nullptr,
            SW_SHOWNORMAL);
    }

    ImGui::Columns(1);

    ImGui::BeginChild("AboutPage##Child");

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::Orange.Value);

    Markdown::Render(u8R"(
## Special thanks
  * [OverShifted](https://github.com/OverShifted) - base author of ImGui theme
  * [MetGang](https://github.com/MetGang) - helped me a lot with C++ templates
  * [MetRiko](https://github.com/Metriko) - helped me with UI design
## Used libraries
  * [fmt](https://github.com/fmtlib/fmt)
  * [imgui](https://github.com/ocornut/imgui)
  * [imgui_markdown](https://github.com/juliettef/imgui_markdown)
  * [json](https://github.com/nlohmann/json)
  * [minhook](https://github.com/TsudaKageyu/minhook)
## Changelog
  * 1.0.4
    * Add support for D-pad
  * 1.0.3
    * Fix crashes related to corrupted config files
  * 1.0.2
    * Add support for Caps Lock key
  * 1.0.1
    * Replace close button with text
  * 1.0
    * First release
)");

    ImGui::PopStyleColor();

    ImGui::EndChild();
}

HRY_NS_END
