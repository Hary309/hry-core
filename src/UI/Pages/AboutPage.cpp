#include "AboutPage.hpp"

#include <string_view>

#include <Windows.h>
#include <fmt/format.h>
#include <imgui.h>
#include <shellapi.h>

#include "Hry/Colors.hpp"
#include "Hry/Fonts.hpp"
#include "Hry/Version.hpp"

#include "UI/Markdown.hpp"

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
    TextCentered(fmt::format("hry-core v{}", ApiVersion.toString()).c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();

    TextCentered(
        fmt::format("Piotr 'Hary309' Krupa <piotrkrupa06@gmail.com>", ApiVersion.toString())
            .c_str());

    ImGui::Columns(3, "AboutPage##Columns", false);

    if (ImGui::Button("Donate", { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "http://paypal.me/PiotrKrupa/1usd", nullptr, nullptr, SW_SHOWNORMAL);
    }

    ImGui::NextColumn();

    if (ImGui::Button("Website", { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "https://hary309.github.io/projects/hry-plugins/", nullptr, nullptr,
            SW_SHOWNORMAL);
    }

    ImGui::NextColumn();

    if (ImGui::Button("Github", { -1, 24 }))
    {
        ShellExecuteA(
            nullptr, "open", "https://github.com/Hary309/hry-core", nullptr, nullptr,
            SW_SHOWNORMAL);
    }

    ImGui::Columns(1);

    ImGui::BeginChild("AboutPage##Child");

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::Orange.Value);

    Markdown::Render(fmt::format(
        u8R"(
## Special thanks
  * [OverShifted](https://github.com/OverShifted) - base author of ImGui theme
  * [MetGang](https://github.com/MetGang) - helped me a lot with C++ templates
  * [Metriko](https://github.com/Metriko) - helped me with UI design
## Used libaries
  * [fmt](https://github.com/fmtlib/fmt)
  * [imgui](https://github.com/ocornut/imgui)
  * [imgui_markdown](https://github.com/juliettef/imgui_markdown)
  * [json](https://github.com/nlohmann/json)
  * [minhook](https://github.com/TsudaKageyu/minhook)
## Changelog
  * 0.1.0.0
    * First release
)",
        ApiVersion.toString()));

    ImGui::PopStyleColor();

    ImGui::EndChild();
}

HRY_NS_END
