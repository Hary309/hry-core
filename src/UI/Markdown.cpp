/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Markdown.hpp"

#include <string>

#include <imgui.h>
#include <imgui_markdown.h>

#include "Hry/Fonts.hpp"

#include "Utils/Windows.hpp"

namespace hry
{
void LinkCallback(ImGui::MarkdownLinkCallbackData linkData)
{
    std::string url(linkData.link, linkData.linkLength);
    if (!linkData.isImage)
    {
        Windows::OpenWebsite(url);
    }
}

void Markdown::Render(std::string_view text)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));

    ImGui::MarkdownConfig config;
    config.linkCallback = LinkCallback;
    config.tooltipCallback = nullptr;
    config.imageCallback = nullptr;
    config.headingFormats[0] = { Fonts::OpenSans_Bold_20, true };
    config.headingFormats[1] = { Fonts::OpenSans_Bold_19, true };
    config.headingFormats[2] = { Fonts::OpenSans_Bold_18, false };
    config.userData = nullptr;

    ImGui::Markdown(text.data(), text.length(), config);

    ImGui::PopStyleVar();
}
}
